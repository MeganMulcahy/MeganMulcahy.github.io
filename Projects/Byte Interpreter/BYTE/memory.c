//memory.c
#include <stdlib.h>

#include "compiler.h"
#include "memory.h"
// #include "processor.h"

#ifdef DEBUG_LOG_GC
#include <stdio.h>
#include "debug.h"
#endif

#define GC_HEAP_GROW_FACTOR 2

void* reallocate(void* pointer, size_t oldSize, size_t newSize) {
  processor.procBytesAlloc += newSize - oldSize;
  if (newSize > oldSize) {
#ifdef DEBUG_STRESS_GC
    trashMan();
#endif

    if (processor.procBytesAlloc > processor.nextGC) {
      trashMan();
    }
  }
  if (newSize == 0) {
    free(pointer);
    return NULL;
  }

  void* result = realloc(pointer, newSize);
  if (result == NULL) exit(1);
  return result;
}

void indicateObject(Obj* object) {
  if (object == NULL) return;
  if (object->isMarked) return;

#ifdef DEBUG_LOG_GC
  printf("%p mark ", (void*)object);
  printValue(OBJECT_VAL(object));
  printf("\n");
#endif

  object->isMarked = true;
  if (processor.grayCapacity < processor.prcoGrayCount + 1) {
    processor.grayCapacity = GROW_CAPACITY(processor.grayCapacity);
    processor.grayStack = (Obj**)realloc(processor.grayStack, sizeof(Obj*) * processor.grayCapacity);
    if (processor.grayStack == NULL) exit(1);
  }
  processor.grayStack[processor.prcoGrayCount++] = object;
}

void indicateValue(Value value) {
  if (IS_OBJ(value)) indicateObject(AS_OBJ(value));
}

static void markArray(ValueArray* array) {
  for (int i = 0; i < array->count; i++) {
    indicateValue(array->values[i]);
  }
}

static void blackenObject(Obj* object) {
#ifdef DEBUG_LOG_GC
  printf("%p blacken ", (void*)object);
  printValue(OBJECT_VAL(object));
  printf("\n");
#endif

  if (object->type == OBJ_TYPE_BOUND_METHOD) {
    ObjMethodLink* bound = (ObjMethodLink*)object;
    indicateValue(bound->receiver);
    indicateObject((Obj*)bound->method);
  } 
  else if (object->type == OBJ_TYPE_CLASS) {
    ObjCategory* objInstanceClass = (ObjCategory*)object;
    indicateObject((Obj*)objInstanceClass->name);
    markReferencesInTable(&objInstanceClass->methods);
  } 
  else if (object->type == OBJ_TYPE_CLOSURE) {
    ObjCompilationUnit* closure = (ObjCompilationUnit*)object;
    indicateObject((Obj*)closure->function);
    for (int i = 0; i < closure->upvalueCount; i++) {
        indicateObject((Obj*)closure->upvalues[i]);
    }
  } 
  else if (object->type == OBJ_TYPE_FUNCTION) {
    ObjFunction* function = (ObjFunction*)object;
    indicateObject((Obj*)function->name);
    markArray(&function->CodeBlock.constants);
  } 
  else if (object->type == OBJ_TYPE_INSTANCE) {
    ObjInstance* instance = (ObjInstance*)object;
    indicateObject((Obj*)instance->objInstanceClass);
    markReferencesInTable(&instance->objInstanceFields);
  } 
  else if (object->type == OBJ_TYPE_UPVALUE) {
    indicateValue(((ObjUpvalue*)object)->closed);
  } 
  else {
  }

}

static void freeObject(Obj* object) {
#ifdef DEBUG_LOG_GC
  printf("%p free type %d\n", (void*)object, object->type);
#endif

 if (object->type == OBJ_TYPE_BOUND_METHOD) {
    FREE(ObjMethodLink, object);
  } 
  else if (object->type == OBJ_TYPE_CLASS) {
    ObjCategory* objInstanceClass = (ObjCategory*)object;
    deallocTable(&objInstanceClass->methods);
    FREE(ObjCategory, object);
  } 
  else if (object->type == OBJ_TYPE_CLOSURE) {
    ObjCompilationUnit* closure = (ObjCompilationUnit*)object;
    FREE_ARRAY(ObjUpvalue*, closure->upvalues, closure->upvalueCount);
    FREE(ObjCompilationUnit, object);
  } 
  else if (object->type == OBJ_TYPE_FUNCTION) {
    ObjFunction* function = (ObjFunction*)object;
    deleteCodeBlock(&function->CodeBlock);
    FREE(ObjFunction, object);
  } 
  else if (object->type == OBJ_TYPE_INSTANCE) {
    ObjInstance* instance = (ObjInstance*)object;
    deallocTable(&instance->objInstanceFields);
    FREE(ObjInstance, object);
  } 
  else if (object->type == OBJ_TYPE_NATIVE) {
    FREE(ObjNativeFunction, object);
  } 
  else if (object->type == OBJ_TYPE_STRING) {
    ObjString* string = (ObjString*)object;
    FREE_ARRAY(char, string->chars, string->stringLength + 1);
    FREE(ObjString, object);
  } 
  else if (object->type == OBJ_TYPE_UPVALUE) {
    FREE(ObjUpvalue, object);
  }

}

static void markRoots() {
  for (Value* slot = processor.procStack; slot < processor.procStackTop; slot++) {
    indicateValue(*slot);
  }
  for (int i = 0; i < processor.procFrameCount; i++) {
    indicateObject((Obj*)processor.frames[i].closure);
  }
  ObjUpvalue* upvalue = processor.openUpvalues;
  while (upvalue != NULL) {
      indicateObject((Obj*)upvalue);
      upvalue = upvalue->nextObjUpvalue;
  }
  markReferencesInTable(&processor.processorGlobals);
  markCompilerRoots();
  indicateObject((Obj*)processor.initString);
}

static void traceReferences() {
  while (processor.prcoGrayCount > 0) {
    Obj* object = processor.grayStack[--processor.prcoGrayCount];
    blackenObject(object);
  }
}

static void sweep() {
  Obj* previous = NULL;
  Obj* object = processor.objects;
  while (object != NULL) {
    if (object->isMarked) {
      object->isMarked = false;
      previous = object;
      object = object->next;
    } else {
      Obj* unreached = object;
      object = object->next;
      if (previous != NULL) {
        previous->next = object;
      } else {
        processor.objects = object;
      }

      freeObject(unreached);
    }
  }
}

void trashMan() {
#ifdef DEBUG_LOG_GC
  printf("-- gc begin\n");
  size_t before = processor.procBytesAlloc;
#endif
  markRoots();
  traceReferences();
  removeEmptyEntries(&processor.processorStrings);
  sweep();
  processor.nextGC = processor.procBytesAlloc * GC_HEAP_GROW_FACTOR;

#ifdef DEBUG_LOG_GC
  printf("-- gc end\n");
  printf("   collected %zu bytes (from %zu to %zu) next at %zu\n",
         before - processor.procBytesAlloc, before, processor.procBytesAlloc,
         processor.nextGC);
#endif
}
void deleteObjects() {
  Obj* object = processor.objects;
  while (object != NULL) {
    Obj* next = object->next;
    freeObject(object);
    object = next;
  }
  free(processor.grayStack);
}
