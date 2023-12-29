//value.c
#include <stdio.h>
#include <string.h>

#include "object.h"
#include "memory.h"
#include "value.h"

void createValueArray(ValueArray* array) {
  array->values = NULL;
  array->capacity = 0;
  array->count = 0;
}

void writeToValueArray(ValueArray* array, Value value) {
  if (array->capacity < array->count + 1) {
    int oldCapacity = array->capacity;
    array->capacity = GROW_CAPACITY(oldCapacity);
    array->values = GROW_ARRAY(Value, array->values, oldCapacity, array->capacity);
  }
  
  array->values[array->count] = value;
  array->count++;
}

void deleteValueArray(ValueArray* array) {
  FREE_ARRAY(Value, array->values, array->capacity);
  createValueArray(array);
}

void printValue(Value value) {
#ifdef NAN_BOXING
  if (IS_BOOL(value)) {
    printf(AS_BOOL(value) ? "true" : "false");
  } else if (IS_NIL(value)) {
    printf("nil");
  } else if (IS_NUMBER(value)) {
    printf("%g", AS_NUMBER(value));
  } else if (IS_OBJ(value)) {
    printObject(value);
  }
#else
  if (value.type == VAL_BOOL) {
    printf(AS_BOOL(value) ? "true" : "false");
  } 
  else if (value.type == VAL_NIL) {
      printf("nil");
  } 
  else if (value.type == VAL_NUMBER) {
      printf("%g", AS_NUMBER(value));
  } 
  else if (value.type == VAL_OBJ) {
      printObject(value);
  }

#endif
}

bool valuesEqual(Value first, Value second) {
#ifdef NAN_BOXING
  if (IS_NUMBER(first) && IS_NUMBER(second)) {
    return AS_NUMBER(first) == AS_NUMBER(second);
  }
  return first == second;
#else
  if (first.type != second.type) return false;
  if (first.type == VAL_BOOL) {
    return AS_BOOL(first) == AS_BOOL(second);
  } 
  else if (first.type == VAL_NIL) {
      return true;
  } 
  else if (first.type == VAL_NUMBER) {
      return AS_NUMBER(first) == AS_NUMBER(second);
  } 
  else if (first.type == VAL_OBJ) {
      return AS_OBJ(first) == AS_OBJ(second);
  } 
  else {
      return false;
  }

#endif
}
