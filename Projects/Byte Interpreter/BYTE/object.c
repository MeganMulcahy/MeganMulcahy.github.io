//object.c
#include <stdio.h>
#include <string.h>

#include "memory.h"
#include "object.h"
#include "table.h"
#include "value.h"
#include "vm.h"

#define ALLOCATE_OBJ(type, objectType) \
    (type*)allocateObject(sizeof(type), objectType)

static Obj* allocateObject(size_t size, Obj_Type type) {
  Obj* object = (Obj*)reallocate(NULL, 0, size);
  object->type = type;
  object->isMarked = false;
  
  object->next = processor.objects;
  processor.objects = object;

#ifdef DEBUG_LOG_GC
  printf("%p allocate %zu for %d\n", (void*)object, size, type);
#endif
  return object;
}

ObjMethodLink* createMethodLink(Value receiver, ObjCompilationUnit* method) {
  ObjMethodLink* bound = ALLOCATE_OBJ(ObjMethodLink, OBJ_TYPE_BOUND_METHOD);
  bound->receiver = receiver;
  bound->method = method;
  return bound;
}

ObjCategory* createClass(ObjString* name) {
  ObjCategory* klass = ALLOCATE_OBJ(ObjCategory, OBJ_TYPE_CLASS);
  klass->name = name;
  createTable(&klass->methods);
  return klass;
}

ObjCompilationUnit* compileBundle(ObjFunction* function) {
  ObjUpvalue** upvalues = ALLOCATE(ObjUpvalue*, function->upvalueCount);
  for (int i = 0; i < function->upvalueCount; i++) {
    upvalues[i] = NULL;
  }
  ObjCompilationUnit* closure = ALLOCATE_OBJ(ObjCompilationUnit, OBJ_TYPE_CLOSURE);
  closure->function = function;
  closure->upvalues = upvalues;
  closure->upvalueCount = function->upvalueCount;
  return closure;
}

ObjFunction* createFunction() {
  ObjFunction* function = ALLOCATE_OBJ(ObjFunction, OBJ_TYPE_FUNCTION);
  function->arity = 0;
  function->upvalueCount = 0;
  function->name = NULL;
  createCodeBlock(&function->CodeBlock);
  return function;
}

ObjInstance* createInstance(ObjCategory* klass) {
  ObjInstance* instance = ALLOCATE_OBJ(ObjInstance, OBJ_TYPE_INSTANCE);
  instance->objInstanceClass = klass;
  createTable(&instance->objInstanceFields);
  return instance;
}

ObjNativeFunction* createNativeFunction(NativeFn function) {
  ObjNativeFunction* native = ALLOCATE_OBJ(ObjNativeFunction, OBJ_TYPE_NATIVE);
  native->function = function;
  return native;
}

static ObjString* allocateString(char* chars, int length, uint32_t hash) {
  ObjString* string = ALLOCATE_OBJ(ObjString, OBJ_TYPE_STRING);
  string->stringLength = length;
  string->hash = hash;
  string->chars = chars;
  pushValue(OBJ_TYPE_VAL(string));
  setTableEntry(&processor.processorStrings, string, NIL_VAL);
  popValue();
  return string;
}

static uint32_t hashString(const char* entryKey, int length) {
  uint32_t hash = 2166136261u;
  for (int i = 0; i < length; i++) {
    hash ^= (uint8_t)entryKey[i];
    hash *= 16777619;
  }
  return hash;
}

ObjString* readInputString(char* chars, int length) {
  uint32_t hash = hashString(chars, length);
  ObjString* interned = findStringInTable(&processor.processorStrings, chars, length, hash);
  if (interned != NULL) {
    FREE_ARRAY(char, chars, length + 1);
    return interned;
  }
  return allocateString(chars, length, hash);
}

ObjString* createString(const char* chars, int length) {
  uint32_t hash = hashString(chars, length);
  ObjString* interned = findStringInTable(&processor.processorStrings, chars, length, hash);
  if (interned != NULL) return interned;
  char* heapChars = ALLOCATE(char, length + 1);
  memcpy(heapChars, chars, length);
  heapChars[length] = '\0';
  return allocateString(heapChars, length, hash);
}

ObjUpvalue* createUpvalue(Value* slot) {
  ObjUpvalue* upvalue = ALLOCATE_OBJ(ObjUpvalue, OBJ_TYPE_UPVALUE);
  upvalue->closed = NIL_VAL;
  upvalue->ObjUpvalueLocation = slot;
  upvalue->nextObjUpvalue = NULL;
  return upvalue;
}

static void printFunction(ObjFunction* function) {
  if (function->name == NULL) {
    printf("<script>");
    return;
  }
  printf("<fn %s>", function->name->chars);
}

void printObject(Value value) {
 if (OBJECT_TYPE(value) == OBJ_TYPE_BOUND_METHOD) {
    printFunction(AS_BOUND_METHOD(value)->method->function);
  } 
  else if (OBJECT_TYPE(value) == OBJ_TYPE_CLASS) {
      printf("%s", AS_CLASS(value)->name->chars);
  } 
  else if (OBJECT_TYPE(value) == OBJ_TYPE_CLOSURE) {
      printFunction(AS_CLOSURE(value)->function);
  } 
  else if (OBJECT_TYPE(value) == OBJ_TYPE_FUNCTION) {
      printFunction(AS_FUNCTION(value));
  } 
  else if (OBJECT_TYPE(value) == OBJ_TYPE_INSTANCE) {
      printf("%s instance", AS_INSTANCE(value)->objInstanceClass->name->chars);
  } 
  else if (OBJECT_TYPE(value) == OBJ_TYPE_NATIVE) {
      printf("<native fn>");
  } 
  else if (OBJECT_TYPE(value) == OBJ_TYPE_STRING) {
      printf("%s", AS_CSTRING(value));
  } 
  else if (OBJECT_TYPE(value) == OBJ_TYPE_UPVALUE) {
      printf("upvalue");
  }

}