//chunk.h
#ifndef clox_codeBlock_h
#define clox_codeBlock_h

#include "common.h"
#include "value.h"

typedef struct {
  int codeBlockCount;
  int codeBlockCapacity;
  uint8_t* byteCode;
  int* byteCodeLineIndex;
  ValueArray constants;
} CodeBlock;

typedef enum {
  CONSTANT,
  NOT, NEGATE, NIL,
  TRUE, FALSE,
  EQUAL, GREATER, LESS,
  ADD, SUBTRACT, MULTIPLY, DIVIDE,
  POP,
  GET_UPVALUE, SET_UPVALUE,
  GET_LOCAL, SET_LOCAL,
  GET_GLOBAL, DEFINE_GLOBAL, SET_GLOBAL,
  GET_PROPERTY, SET_PROPERTY,
  GET_SUPER,
  CLASS, INHERIT, METHOD,
  PRINT,
  JUMP, JUMP_IF_FALSE,
  CLOSURE, CLOSE_UPVALUE,
  LOOP,
  CALL,
  INVOKE, SUPER_INVOKE,
  RETURN,
} OpCode;

void createCodeBlock(CodeBlock* codeBlock);

void writeToCodeBlock(CodeBlock* codeBlock, uint8_t byte, int line);

void deleteCodeBlock(CodeBlock* codeBlock);

int appendLiteral(CodeBlock* codeBlock, Value entryValue);

#endif
