//vm.c processor.c
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "common.h"
#include "compiler.h"
#include "debug.h"
#include "object.h"
#include "memory.h"
#include "vm.h"

VM processor; 

static Value clockNative(int argCount, Value* args) {
  return NUMBER_VAL((double)clock() / CLOCKS_PER_SEC);
}

static void resetStack() {
  processor.procStackTop = processor.procStack;
  processor.procFrameCount = 0;
  processor.openUpvalues = NULL;
}

static void reportRuntimeError(const char* format, ...) {
  va_list args;
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);
  fputs("\n", stderr);
  int i = processor.procFrameCount - 1;
  while (i >= 0) {
    CallFrame* frame = &processor.frames[i];
    ObjFunction* function = frame->closure->function;
    size_t instruction = frame->ip - function->CodeBlock.byteCode - 1;
    fprintf(stderr, "[line %d] in ", function->CodeBlock.byteCodeLineIndex[instruction]);
    if (function->name == NULL) {
        fprintf(stderr, "script\n");
    } else {
        fprintf(stderr, "%s()\n", function->name->chars);
    }
    i--;
  }
  resetStack();
}

static void defineNative(const char* name, NativeFn function) {
  pushValue(OBJ_TYPE_VAL(createString(name, (int)strlen(name))));
  pushValue(OBJ_TYPE_VAL(createNativeFunction(function)));
  setTableEntry(&processor.processorGlobals, AS_STRING(processor.procStack[0]), processor.procStack[1]);
  popValue();
  popValue();
}

void createProcessor() {
  resetStack();
  processor.objects = NULL;
  processor.procBytesAlloc = 0;
  processor.nextGC = 1024 * 1024;
  processor.prcoGrayCount = 0;
  processor.grayCapacity = 0;
  processor.grayStack = NULL;

  createTable(&processor.processorGlobals);
  createTable(&processor.processorStrings);

  processor.initString = NULL;
  processor.initString = createString("init", 4);

  defineNative("clock", clockNative);
}

void deleteProcessor() {
  deallocTable(&processor.processorGlobals);
  deallocTable(&processor.processorStrings);
  processor.initString = NULL;
  deleteObjects();
}

void pushValue(Value value) {
  *processor.procStackTop = value;
  processor.procStackTop++;
}

Value popValue() {
  processor.procStackTop--;
  return *processor.procStackTop;
}

Value peekValue(int distance) {
  return processor.procStackTop[-1 - distance];
}

static bool call(ObjCompilationUnit* closure, int argCount){
  if (argCount != closure->function->arity) {
    reportRuntimeError("Expected %d arguments but got %d.",
        closure->function->arity, argCount);
    return false;
  }
  if (processor.procFrameCount == FRAMES_MAX) {
    reportRuntimeError("Stack overflow.");
    return false;
  }

  CallFrame* frame = &processor.frames[processor.procFrameCount++];
  frame->closure = closure;
  frame->ip = closure->function->CodeBlock.byteCode;
  frame->slots = processor.procStackTop - argCount - 1;
  return true;
}

bool callValue(Value callee, int argCount) {
  if (IS_OBJ(callee)) {
    if (OBJECT_TYPE(callee) == OBJ_TYPE_BOUND_METHOD) {
    ObjMethodLink* bound = AS_BOUND_METHOD(callee);
    processor.procStackTop[-argCount - 1] = bound->receiver;
    return call(bound->method, argCount);
  } 
  else if (OBJECT_TYPE(callee) == OBJ_TYPE_CLASS) {
      ObjCategory* objInstanceClass = AS_CLASS(callee);
      processor.procStackTop[-argCount - 1] = OBJ_TYPE_VAL(createInstance(objInstanceClass));
      Value initializer;
      if (findTableEntry(&objInstanceClass->methods, processor.initString, &initializer)) {
          return call(AS_CLOSURE(initializer), argCount);
      } else if (argCount != 0) {
          reportRuntimeError("Expected 0 arguments but got %d.", argCount);
          return false;
      }
      return true;
  } 
  else if (OBJECT_TYPE(callee) == OBJ_TYPE_CLOSURE) {
      return call(AS_CLOSURE(callee), argCount);
  } 
  else if (OBJECT_TYPE(callee) == OBJ_TYPE_NATIVE) {
      NativeFn native = AS_NATIVE(callee);
      Value result = native(argCount, processor.procStackTop - argCount);
      processor.procStackTop -= argCount + 1;
      pushValue(result);
      return true;
  }

  }
  reportRuntimeError("Can only call functions and classes.");
  return false;
}

static bool invokeFromClass(ObjCategory* klass, ObjString* name, int argCount) {
  Value method;
  if (!findTableEntry(&klass->methods, name, &method)) {
    reportRuntimeError("Undefined property '%s'.", name->chars);
    return false;
  }
  return call(AS_CLOSURE(method), argCount);
}

static bool invoke(ObjString* name, int argCount) {
  Value receiver = peekValue(argCount);

  if (!IS_INSTANCE(receiver)) {
    reportRuntimeError("Only instances have methods.");
    return false;
  }

  ObjInstance* instance = AS_INSTANCE(receiver);
  Value entryValue;
  if (findTableEntry(&instance->objInstanceFields, name, &entryValue)) {
    processor.procStackTop[-argCount - 1] = entryValue;
    return callValue(entryValue, argCount);
  }
  return invokeFromClass(instance->objInstanceClass, name, argCount);
}

static bool bindMethod(ObjCategory* klass, ObjString* name) {
  Value method;
  if (!findTableEntry(&klass->methods, name, &method)) {
    reportRuntimeError("Undefined property '%s'.", name->chars);
    return false;
  }

  ObjMethodLink* bound = createMethodLink(peekValue(0), AS_CLOSURE(method));
  popValue();
  pushValue(OBJ_TYPE_VAL(bound));
  return true;
}

static ObjUpvalue* captureUpvalue(Value* local) {
  ObjUpvalue* prevUpvalue = NULL;
  ObjUpvalue* upvalue = processor.openUpvalues;
  while (upvalue != NULL && upvalue->ObjUpvalueLocation > local) {
    prevUpvalue = upvalue;
    upvalue = upvalue->nextObjUpvalue;
  }

  if (upvalue != NULL && upvalue->ObjUpvalueLocation == local) {
    return upvalue;
  }
  ObjUpvalue* createdUpvalue = createUpvalue(local);
  createdUpvalue->nextObjUpvalue = upvalue;

  if (prevUpvalue == NULL) {
    processor.openUpvalues = createdUpvalue;
  } else {
    prevUpvalue->nextObjUpvalue = createdUpvalue;
  }
  return createdUpvalue;
}

static void closeUpvalues(Value* last) {
  while (processor.openUpvalues != NULL &&
         processor.openUpvalues->ObjUpvalueLocation >= last) {
    ObjUpvalue* upvalue = processor.openUpvalues;
    upvalue->closed = *upvalue->ObjUpvalueLocation;
    upvalue->ObjUpvalueLocation = &upvalue->closed;
    processor.openUpvalues = upvalue->nextObjUpvalue;
  }
}

static void defineMethod(ObjString* name) {
  Value method = peekValue(0);
  ObjCategory* klass = AS_CLASS(peekValue(1));
  setTableEntry(&klass->methods, name, method);
  popValue();
}

static bool isFalsey(Value entryValue) {
  return IS_NIL(entryValue) || (IS_BOOL(entryValue) && !AS_BOOL(entryValue));
}

static void concatenate() {
  ObjString* second = AS_STRING(peekValue(0));
  ObjString* first = AS_STRING(peekValue(1));

  int length = first->stringLength + second->stringLength;
  char* chars = ALLOCATE(char, length + 1);
  memcpy(chars, first->chars, first->stringLength);
  memcpy(chars + first->stringLength, second->chars, second->stringLength);
  chars[length] = '\0';

  ObjString* result = readInputString(chars, length);
  popValue();
  popValue();
  pushValue(OBJ_TYPE_VAL(result));
}

static Interpreter run() {
  CallFrame* frame = &processor.frames[processor.procFrameCount - 1];
#define READ_BYTE() (*frame->ip++)
#define READ_SHORT() \
    (frame->ip += 2, \
    (uint16_t)((frame->ip[-2] << 8) | frame->ip[-1]))
#define READ_CONSTANT() \
    (frame->closure->function->CodeBlock.constants.values[READ_BYTE()])
#define READ_STRING() AS_STRING(READ_CONSTANT())
#define BINARY_OP(valueType, op) \
    do { \
      if (!IS_NUMBER(peekValue(0)) || !IS_NUMBER(peekValue(1))) { \
        reportRuntimeError("Operands must be numbers."); \
        return INTERPRET_RUNTIME_ERROR; \
      } \
      double second = AS_NUMBER(popValue()); \
      double first = AS_NUMBER(popValue()); \
      pushValue(valueType(first op second)); \
    } while (false)

  for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
    printf("          ");
    for (Value* slot = processor.procStack; slot < processor.procStackTop; slot++) {
      printf("[ ");
      printValue(*slot);
      printf(" ]");
    }
    printf("\n");
    runInstructionDebugPrints(&frame->closure->function->CodeBlock,
        (int)(frame->ip - frame->closure->function->CodeBlock.byteCode));
#endif
    uint8_t instruction;
    instruction = READ_BYTE();
    if (instruction == CONSTANT) {
        Value constant = READ_CONSTANT();
        pushValue(constant);
    } else if (instruction == NIL) {
        pushValue(NIL_VAL);
    } else if (instruction == TRUE) {
        pushValue(BOOL_VAL(true));
    } else if (instruction == FALSE) {
        pushValue(BOOL_VAL(false));
    } else if (instruction == POP) {
        popValue();
    } else if (instruction == GET_LOCAL) {
        uint8_t slot = READ_BYTE();
        pushValue(frame->slots[slot]);
    } else if (instruction == SET_LOCAL) {
        uint8_t slot = READ_BYTE();
        frame->slots[slot] = peekValue(0);
    } else if (instruction == GET_GLOBAL) {
        ObjString* name = READ_STRING();
        Value entryValue;
        if (!findTableEntry(&processor.processorGlobals, name, &entryValue)) {
            reportRuntimeError("Undefined variable '%s'.", name->chars);
            return INTERPRET_RUNTIME_ERROR;
        }
        pushValue(entryValue);
    } else if (instruction == DEFINE_GLOBAL) {
        ObjString* name = READ_STRING();
        setTableEntry(&processor.processorGlobals, name, peekValue(0));
        popValue();
    } else if (instruction == SET_GLOBAL) {
        ObjString* name = READ_STRING();
        if (setTableEntry(&processor.processorGlobals, name, peekValue(0))) {
            deleteTableEntry(&processor.processorGlobals, name);
            reportRuntimeError("Undefined variable '%s'.", name->chars);
            return INTERPRET_RUNTIME_ERROR;
        }
    } else if (instruction == GET_UPVALUE) {
        uint8_t slot = READ_BYTE();
        pushValue(*frame->closure->upvalues[slot]->ObjUpvalueLocation);
    } else if (instruction == SET_UPVALUE) {
        uint8_t slot = READ_BYTE();
        *frame->closure->upvalues[slot]->ObjUpvalueLocation = peekValue(0);
    } else if (instruction == GET_PROPERTY) {
        if (!IS_INSTANCE(peekValue(0))) {
            reportRuntimeError("Only instances have properties.");
            return INTERPRET_RUNTIME_ERROR;
        }
        ObjInstance* instance = AS_INSTANCE(peekValue(0));
        ObjString* name = READ_STRING();
        Value entryValue;
        if (findTableEntry(&instance->objInstanceFields, name, &entryValue)) {
            popValue();
            pushValue(entryValue);
        } else {
            if (!bindMethod(instance->objInstanceClass, name)) {
                return INTERPRET_RUNTIME_ERROR;
            }
        }
    } else if (instruction == SET_PROPERTY) {
        if (!IS_INSTANCE(peekValue(1))) {
            reportRuntimeError("Only instances have fields.");
            return INTERPRET_RUNTIME_ERROR;
        }
        ObjInstance* instance = AS_INSTANCE(peekValue(1));
        setTableEntry(&instance->objInstanceFields, READ_STRING(), peekValue(0));
        Value entryValue = popValue();
        popValue();
        pushValue(entryValue);
    } else if (instruction == GET_SUPER) {
        ObjString* name = READ_STRING();
        ObjCategory* superclass = AS_CLASS(popValue());
        if (!bindMethod(superclass, name)) {
            return INTERPRET_RUNTIME_ERROR;
        }
    } else if (instruction == EQUAL) {
        Value second = popValue();
        Value first = popValue();
        pushValue(BOOL_VAL(valuesEqual(first, second)));
    } else if (instruction == GREATER) {
        BINARY_OP(BOOL_VAL, >);
    } else if (instruction == LESS) {
        BINARY_OP(BOOL_VAL, <);
    } else if (instruction == ADD) {
        if (IS_STRING(peekValue(0)) && IS_STRING(peekValue(1))) {
            concatenate();
        } else if (IS_NUMBER(peekValue(0)) && IS_NUMBER(peekValue(1))) {
            double second = AS_NUMBER(popValue());
            double first = AS_NUMBER(popValue());
            pushValue(NUMBER_VAL(first + second));
        } else {
            reportRuntimeError("Operands must be two numbers or two strings.");
            return INTERPRET_RUNTIME_ERROR;
        }
    } else if (instruction == SUBTRACT) {
        BINARY_OP(NUMBER_VAL, -);
    } else if (instruction == MULTIPLY) {
        BINARY_OP(NUMBER_VAL, *);
    } else if (instruction == DIVIDE) {
        BINARY_OP(NUMBER_VAL, /);
    } else if (instruction == NOT) {
        pushValue(BOOL_VAL(isFalsey(popValue())));
    } else if (instruction == NEGATE) {
        if (!IS_NUMBER(peekValue(0))) {
            reportRuntimeError("Operand must be a number.");
            return INTERPRET_RUNTIME_ERROR;
        }
        pushValue(NUMBER_VAL(-AS_NUMBER(popValue())));
    } else if (instruction == PRINT) {
        printValue(popValue());
        printf("\n");
    } else if (instruction == JUMP) {
        uint16_t offset = READ_SHORT();
        frame->ip += offset;
    } else if (instruction == JUMP_IF_FALSE) {
        uint16_t offset = READ_SHORT();
        if (isFalsey(peekValue(0))) frame->ip += offset;
    } else if (instruction == LOOP) {
        uint16_t offset = READ_SHORT();
        frame->ip -= offset;
    } else if (instruction == CALL) {
        int argCount = READ_BYTE();
        if (!callValue(peekValue(argCount), argCount)) {
            return INTERPRET_RUNTIME_ERROR;
        }
        frame = &processor.frames[processor.procFrameCount - 1];
    } else if (instruction == INVOKE) {
        ObjString* method = READ_STRING();
        int argCount = READ_BYTE();
        if (!invoke(method, argCount)) {
            return INTERPRET_RUNTIME_ERROR;
        }
        frame = &processor.frames[processor.procFrameCount - 1];
    } else if (instruction == SUPER_INVOKE) {
        ObjString* method = READ_STRING();
        int argCount = READ_BYTE();
        ObjCategory* superclass = AS_CLASS(popValue());
        if (!invokeFromClass(superclass, method, argCount)) {
            return INTERPRET_RUNTIME_ERROR;
        }
        frame = &processor.frames[processor.procFrameCount - 1];
    } else if (instruction == CLOSURE) {
        ObjFunction* function = AS_FUNCTION(READ_CONSTANT());
        ObjCompilationUnit* closure = compileBundle(function);
        pushValue(OBJ_TYPE_VAL(closure));
        for (int i = 0; i < closure->upvalueCount; i++) {
            uint8_t isLocal = READ_BYTE();
            uint8_t index = READ_BYTE();
            if (isLocal) {
                closure->upvalues[i] = captureUpvalue(frame->slots + index);
            } else {
                closure->upvalues[i] = frame->closure->upvalues[index];
            }
        }
    } else if (instruction == CLOSE_UPVALUE) {
        closeUpvalues(processor.procStackTop - 1);
        popValue();
    } else if (instruction == RETURN) {
        Value result = popValue();
        closeUpvalues(frame->slots);
        processor.procFrameCount--;
        if (processor.procFrameCount == 0) {
            popValue();
            return INTERPRET_OK;
        }
        processor.procStackTop = frame->slots;
        pushValue(result);
        frame = &processor.frames[processor.procFrameCount - 1];
    } else if (instruction == CLASS) {
        pushValue(OBJ_TYPE_VAL(createClass(READ_STRING())));
    } else if (instruction == INHERIT) {
        Value superclass = peekValue(1);
        if (!IS_CLASS(superclass)) {
            reportRuntimeError("Superclass must be a class.");
            return INTERPRET_RUNTIME_ERROR;
        }
        ObjCategory* subclass = AS_CLASS(peekValue(0));
        mergeTables(&AS_CLASS(superclass)->methods, &subclass->methods);
        popValue();
    } else if (instruction == METHOD) {
        defineMethod(READ_STRING());
    }
  }

#undef READ_BYTE
#undef READ_SHORT
#undef READ_CONSTANT
#undef READ_STRING
#undef BINARY_OP
}

void hack(bool second) {
  run();
  if (second) hack(false);
}

Interpreter interpret(const char* root) {
  ObjFunction* function = compile(root);
  if (function == NULL) return INTERPRET_COMPILE_ERROR;
  pushValue(OBJ_TYPE_VAL(function));
  ObjCompilationUnit* closure = compileBundle(function);
  popValue();
  pushValue(OBJ_TYPE_VAL(closure));
  call(closure, 0);
  return run();
}
