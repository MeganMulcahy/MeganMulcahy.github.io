//chunk.c
#include <stdlib.h>

#include "chunk.h"
#include "memory.h"
#include "vm.h"

void createCodeBlock(CodeBlock* codeBlock) {
  codeBlock->byteCode = NULL;
  codeBlock->byteCodeLineIndex = NULL;
  codeBlock->codeBlockCount = 0;
  codeBlock->codeBlockCapacity = 0;
  createValueArray(&codeBlock->constants);
}

void writeToCodeBlock(CodeBlock* codeBlock, uint8_t byte, int line) {
  if (codeBlock->codeBlockCapacity < codeBlock->codeBlockCount + 1) {
    int oldCapacity = codeBlock->codeBlockCapacity;
    codeBlock->codeBlockCapacity = GROW_CAPACITY(oldCapacity);
    codeBlock->byteCode = GROW_ARRAY(uint8_t, codeBlock->byteCode, oldCapacity, codeBlock->codeBlockCapacity);
    codeBlock->byteCodeLineIndex = GROW_ARRAY(int, codeBlock->byteCodeLineIndex, oldCapacity, codeBlock->codeBlockCapacity);
  }
  codeBlock->byteCodeLineIndex[codeBlock->codeBlockCount] = line;
  codeBlock->byteCode[codeBlock->codeBlockCount] = byte;
  codeBlock->codeBlockCount++;
}

void deleteCodeBlock(CodeBlock* codeBlock) {
  FREE_ARRAY(uint8_t, codeBlock->byteCode, codeBlock->codeBlockCapacity);
  FREE_ARRAY(int, codeBlock->byteCodeLineIndex, codeBlock->codeBlockCapacity);
  deleteValueArray(&codeBlock->constants);
  createCodeBlock(codeBlock);
}

int appendLiteral(CodeBlock* codeBlock, Value entryValue) {
  pushValue(entryValue);
  writeToValueArray(&codeBlock->constants, entryValue);
  popValue();
  return codeBlock->constants.count - 1;
}
