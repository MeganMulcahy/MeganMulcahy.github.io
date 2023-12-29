//debug.c
#include <stdio.h>

#include "debug.h"
#include "object.h"
#include "value.h"

static int printInstruction(const char* name, int offset) {
  printf("%s\n", name);
  return offset + 1;
}

static int printByteInstruction(const char* name, CodeBlock* codeBlock, int offset) {
  uint8_t slot = codeBlock->byteCode[offset + 1];
  printf("%-16s %4d\n", name, slot);
  return offset + 2;
}

static int instruction(const char* name, CodeBlock* codeBlock, int offset) {
  uint8_t constant = codeBlock->byteCode[offset + 1];
  printf("%-16s %4d '", name, constant);
  printValue(codeBlock->constants.values[constant]);
  printf("'\n");
  return offset + 2;
}

static int printJumpInstruction(const char* name, int sign, CodeBlock* codeBlock, int offset) {
  uint16_t jump = (uint16_t)(codeBlock->byteCode[offset + 1] << 8);
  jump |= codeBlock->byteCode[offset + 2];
  printf("%-16s %4d -> %d\n", name, offset,
         offset + 3 + sign * jump);
  return offset + 3;
}

static int useInstruction(const char* name, CodeBlock* codeBlock, int offset) {
  uint8_t constant = codeBlock->byteCode[offset + 1];
  uint8_t argCount = codeBlock->byteCode[offset + 2];
  printf("%-16s (%d args) %4d '", name, argCount, constant);
  printValue(codeBlock->constants.values[constant]);
  printf("'\n");
  return offset + 3;
}

int runInstructionDebugPrints(CodeBlock* codeBlock, int offset) {
  printf("%04d ", offset);
  if (offset > 0 && codeBlock->byteCodeLineIndex[offset] == codeBlock->byteCodeLineIndex[offset - 1]) {
    printf("   | ");
  } 
  else {
    printf("%4d ", codeBlock->byteCodeLineIndex[offset]);
  }
  
  uint8_t instructionOperation = codeBlock->byteCode[offset];
  if (instructionOperation == CONSTANT) {
    return instruction("CONSTANT", codeBlock, offset);
    } 
  else if (instructionOperation == NIL) {
    return printInstruction("NIL", offset);
    } 
  else if (instructionOperation == TRUE) {
    return printInstruction("TRUE", offset);
    } 
  else if (instructionOperation == FALSE) {
    return printInstruction("FALSE", offset);
    } 
  else if (instructionOperation == POP) {
    return printInstruction("POP", offset);
    } 
  else if (instructionOperation == GET_LOCAL) {
    return printByteInstruction("GET_LOCAL", codeBlock, offset);
    } 
  else if (instructionOperation == SET_LOCAL) {
    return printByteInstruction("SET_LOCAL", codeBlock, offset);
    } 
  else if (instructionOperation == GET_GLOBAL) {
    return instruction("GET_GLOBAL", codeBlock, offset);
    } 
  else if (instructionOperation == DEFINE_GLOBAL) {
   return instruction("DEFINE_GLOBAL", codeBlock, offset);
    } 
  else if (instructionOperation == SET_GLOBAL) {
    return instruction("SET_GLOBAL", codeBlock, offset);
    } 
  else if (instructionOperation == GET_UPVALUE) {
    return printByteInstruction("GET_UPVALUE", codeBlock, offset);
    } 
  else if (instructionOperation == SET_UPVALUE) {
    return printByteInstruction("SET_UPVALUE", codeBlock, offset);
    } 
  else if (instructionOperation == GET_PROPERTY) {
    return instruction("GET_PROPERTY", codeBlock, offset);
    } 
  else if (instructionOperation == SET_PROPERTY) {
    return instruction("SET_PROPERTY", codeBlock, offset);
    } 
  else if (instructionOperation == GET_SUPER) {
    return instruction("GET_SUPER", codeBlock, offset);
    } 
  else if (instructionOperation == EQUAL) {
    return printInstruction("EQUAL", offset);
    } 
  else if (instructionOperation == GREATER) {
    return printInstruction("GREATER", offset);
    } 
  else if (instructionOperation == LESS) {
    return printInstruction("LESS", offset);
    } 
  else if (instructionOperation == ADD) {
    return printInstruction("ADD", offset);
    } 
  else if (instructionOperation == SUBTRACT) {
    return printInstruction("SUBTRACT", offset);
    } 
  else if (instructionOperation == MULTIPLY) {
    return printInstruction("MULTIPLY", offset);
    } 
  else if (instructionOperation == DIVIDE) {
    return printInstruction("DIVIDE", offset);
    } 
  else if (instructionOperation == NOT) {
    return printInstruction("NOT", offset);
    } 
  else if (instructionOperation == NEGATE) {
    return printInstruction("NEGATE", offset);
    } 
  else if (instructionOperation == PRINT) {
    return printInstruction("PRINT", offset);
    } 
  else if (instructionOperation == JUMP) {
    return printJumpInstruction("JUMP", 1, codeBlock, offset);
    } 
  else if (instructionOperation == JUMP_IF_FALSE) {
    return printJumpInstruction("JUMP_IF_FALSE", 1, codeBlock, offset);
    } 
  else if (instructionOperation == LOOP) {
    return printJumpInstruction("LOOP", -1, codeBlock, offset);
    } 
  else if (instructionOperation == CALL) {
    return printByteInstruction("CALL", codeBlock, offset);
    } 
  else if (instructionOperation == INVOKE) {
    return useInstruction("INVOKE", codeBlock, offset);
    } 
  else if (instructionOperation == SUPER_INVOKE) {
    return useInstruction("SUPER_INVOKE", codeBlock, offset);
    } 
  else if (instructionOperation == CLOSURE) {
    offset++;
    uint8_t constant = codeBlock->byteCode[offset++];
    printf("%-16s %4d ", "CLOSURE", constant);
    printValue(codeBlock->constants.values[constant]);
    printf("\n");
    ObjFunction* function = AS_FUNCTION(
        codeBlock->constants.values[constant]);
    int j = 0;
    while (j < function->upvalueCount) {
        int isLocal = codeBlock->byteCode[offset++];
        int index = codeBlock->byteCode[offset++];
        printf("%04d      |                     %s %d\n",
              offset - 2, isLocal ? "local" : "upvalue", index);
        j++;
    }
    return offset;
    } 
  else if (instructionOperation == CLOSE_UPVALUE) {
    return printInstruction("CLOSE_UPVALUE", offset);
    } 
  else if (instructionOperation == RETURN) {
    return printInstruction("RETURN", offset);
    } 
  else if (instructionOperation == CLASS) {
    return instruction("CLASS", codeBlock, offset);
    } 
  else if (instructionOperation == INHERIT) {
    return printInstruction("INHERIT", offset);
    } 
  else if (instructionOperation == METHOD) {
    return instruction("METHOD", codeBlock, offset);
    } else {
    printf("Unknown Operations %d\n", instructionOperation);
    return offset + 1;
    }
}

void destroyCodeBlock(CodeBlock* codeBlock, const char* name) {
  printf("== %s ==\n", name);
  int offset = 0;
  while (offset < codeBlock->codeBlockCount) {
      offset = runInstructionDebugPrints(codeBlock, offset);
  }
}