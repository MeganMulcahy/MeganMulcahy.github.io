//object.g
#ifndef clox_object_h
#define clox_object_h

#include "common.h"
#include "chunk.h"
#include "table.h"
#include "value.h"

#define OBJECT_TYPE(value)        (AS_OBJ(value)->type)
#define IS_BOUND_METHOD(value) isObjType(value, OBJ_TYPE_BOUND_METHOD)
#define IS_CLASS(value)        isObjType(value, OBJ_TYPE_CLASS)
#define IS_CLOSURE(value)      isObjType(value, OBJ_TYPE_CLOSURE)
#define IS_FUNCTION(value)     isObjType(value, OBJ_TYPE_FUNCTION)
#define IS_INSTANCE(value)     isObjType(value, OBJ_TYPE_INSTANCE)
#define IS_NATIVE(value)       isObjType(value, OBJ_TYPE_NATIVE)
#define IS_STRING(value)       isObjType(value, OBJ_TYPE_STRING)
#define AS_BOUND_METHOD(value) ((ObjMethodLink*)AS_OBJ(value))
#define AS_CLASS(value)        ((ObjCategory*)AS_OBJ(value))
#define AS_CLOSURE(value)      ((ObjCompilationUnit*)AS_OBJ(value))
#define AS_FUNCTION(value)     ((ObjFunction*)AS_OBJ(value))
#define AS_INSTANCE(value)     ((ObjInstance*)AS_OBJ(value))
#define AS_NATIVE(value) \
    (((ObjNativeFunction*)AS_OBJ(value))->function)
#define AS_STRING(value)       ((ObjString*)AS_OBJ(value))
#define AS_CSTRING(value)      (((ObjString*)AS_OBJ(value))->chars)

typedef enum {
  OBJ_TYPE_BOUND_METHOD,
  OBJ_TYPE_CLASS,
  OBJ_TYPE_CLOSURE,
  OBJ_TYPE_FUNCTION,
  OBJ_TYPE_INSTANCE,
  OBJ_TYPE_NATIVE,
  OBJ_TYPE_STRING,
  OBJ_TYPE_UPVALUE
} Obj_Type;

struct Obj {
  Obj_Type type;
  bool isMarked;
  struct Obj* next;
};

typedef struct {
  Obj obj_Func_Obj;
  int arity;
  int upvalueCount;
  CodeBlock CodeBlock;
  ObjString* name;
} ObjFunction;

typedef Value (*NativeFn)(int argCount, Value* args);

typedef struct {
  Obj ObjNativeFunctionObject;
  NativeFn function;
} ObjNativeFunction;

struct ObjString {
  Obj obj;
  int stringLength;
  char* chars;
  uint32_t hash;
};

typedef struct ObjUpvalue {
  Obj ObjUpvalueObject;
  Value* ObjUpvalueLocation;
  Value closed;
  struct ObjUpvalue* nextObjUpvalue;
} ObjUpvalue;

typedef struct {
  Obj ObjCompilationUnitObject;
  ObjFunction* function;
  ObjUpvalue** upvalues;
  int upvalueCount;
} ObjCompilationUnit;

typedef struct {
  Obj ObjCategoryObject;
  ObjString* name;
  Table methods;
} ObjCategory;

typedef struct {
  Obj ObjInstanceObject;
  ObjCategory* objInstanceClass;
  Table objInstanceFields;
} ObjInstance;

typedef struct {
  Obj ObjMethodLinkObject;
  Value receiver;
  ObjCompilationUnit* method;
} ObjMethodLink;

ObjMethodLink* createMethodLink(Value receiver, ObjCompilationUnit* method);

ObjCategory* createClass(ObjString* name);

ObjCompilationUnit* compileBundle(ObjFunction* function);

ObjFunction* createFunction();

ObjInstance* createInstance(ObjCategory* objInstanceClass);

ObjNativeFunction* createNativeFunction(NativeFn function);

ObjString* readInputString(char* chars, int length);

ObjString* createString(const char* chars, int length);

ObjUpvalue* createUpvalue(Value* slot);

void printObject(Value value);

static inline bool isObjType(Value value, Obj_Type type) {
  return IS_OBJ(value) && AS_OBJ(value)->type == type;
}
#endif
