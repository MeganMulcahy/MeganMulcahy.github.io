//processor.h
#ifndef clox_processor_h
#define clox_processor_h

#include "object.h"
#include "table.h"
#include "value.h"

#define FRAMES_MAX 64
#define STACK_MAX (FRAMES_MAX * UINT8_COUNT)

typedef struct {
  ObjCompilationUnit* closure;
  uint8_t* ip;
  Value* slots;
} CallFrame;

typedef struct {
  CallFrame frames[FRAMES_MAX];
  int procFrameCount;
  Value procStack[STACK_MAX];
  Value* procStackTop;
  Table processorGlobals;
  Table processorStrings;
  ObjString* initString;
  ObjUpvalue* openUpvalues;
  size_t procBytesAlloc;
  size_t nextGC;
  Obj* objects;
  int prcoGrayCount;
  int grayCapacity;
  Obj** grayStack;
} VM;

typedef enum {
  INTERPRET_OK,
  INTERPRET_COMPILE_ERROR,
  INTERPRET_RUNTIME_ERROR
} Interpreter;

extern VM processor;

void createProcessor();

void deleteProcessor();

bool bindMethodToCategory(ObjCategory* objInstanceClass, ObjString* name);

Interpreter interpret(const char* root);


void pushValue(Value value);

Value popValue();

bool invokeMethod(ObjString* method, int argCount);

bool invokeMethodFromClass(ObjCategory* objInstanceClass, ObjString* method, int argCount);

#endif
