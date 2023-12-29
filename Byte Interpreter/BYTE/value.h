// value.h
#ifndef clox_value_h
#define clox_value_h
#include <string.h>

#include "common.h"

typedef struct Obj Obj;
typedef struct ObjString ObjString;

#ifdef NAN_BOXING
#define SIGN_BIT ((uint64_t)0x8000000000000000)
#define QNAN     ((uint64_t)0x7ffc000000000000)
#define TAG_NIL   1
#define TAG_FALSE 2
#define TAG_TRUE  3 

typedef uint64_t Value;

#define IS_BOOL(entryValue)      (((entryValue) | 1) == TRUE_VAL)
#define IS_NIL(entryValue)       ((entryValue) == NIL_VAL)
#define IS_NUMBER(entryValue)    (((entryValue) & QNAN) != QNAN)
#define IS_OBJ(entryValue) \
    (((entryValue) & (QNAN | SIGN_BIT)) == (QNAN | SIGN_BIT))
#define AS_BOOL(entryValue)      ((entryValue) == TRUE_VAL)
#define AS_NUMBER(entryValue)    valueToNumberConverter(entryValue)
#define AS_OBJ(entryValue) \
    ((Obj*)(uintptr_t)((entryValue) & ~(SIGN_BIT | QNAN)))
#define BOOL_VAL(second)     ((second) ? TRUE_VAL : FALSE_VAL)
#define FALSE_VAL       ((Value)(uint64_t)(QNAN | TAG_FALSE))
#define TRUE_VAL        ((Value)(uint64_t)(QNAN | TAG_TRUE))
#define NIL_VAL         ((Value)(uint64_t)(QNAN | TAG_NIL))
#define NUMBER_VAL(num) numberToValueConverter(num)
#define OBJ_TYPE_VAL(obj) \
    (Value)(SIGN_BIT | QNAN | (uint64_t)(uintptr_t)(obj))

static inline double valueToNumberConverter(Value value) {
  double num;
  memcpy(&num, &value, sizeof(Value));
  return num;
}

static inline Value numberToValueConverter(double num) {
  Value value;
  memcpy(&value, &num, sizeof(double));
  return value;
}

#else

typedef enum {
  VAL_BOOL,
  VAL_NIL,
  VAL_NUMBER,
  VAL_OBJ
} ValueType;

typedef struct {
  ValueType type;
  union {
    bool boolean;
    double number;
    Obj* obj;
  } as;
} Value;

#define IS_BOOL(value)    ((value).type == VAL_BOOL)
#define IS_NIL(value)     ((value).type == VAL_NIL)
#define IS_NUMBER(value)  ((value).type == VAL_NUMBER)
#define IS_OBJ(value)     ((value).type == VAL_OBJ)
#define AS_OBJ(value)     ((value).as.obj)
#define AS_BOOL(value)    ((value).as.boolean)
#define AS_NUMBER(value)  ((value).as.number)
#define BOOL_VAL(value)   ((Value){VAL_BOOL, {.boolean = value}})
#define NIL_VAL           ((Value){VAL_NIL, {.number = 0}})
#define NUMBER_VAL(value) ((Value){VAL_NUMBER, {.number = value}})
#define OBJECT_VAL(object)   ((Value){VAL_OBJ, {.obj = (Obj*)object}})

#endif

typedef struct {
  int capacity;
  int count;
  Value* values;
} ValueArray;

bool valuesEqual(Value first, Value second);

void createValueArray(ValueArray* array);

void writeToValueArray(ValueArray* array, Value value);

void deleteValueArray(ValueArray* array);

void printValue(Value value);

#endif
