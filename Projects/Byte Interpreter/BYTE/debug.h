//> Chunks of Bytecode debug-h
#ifndef clox_debug_h
#define clox_debug_h

#include "chunk.h"

int runInstructionDebugPrints(CodeBlock* CodeBlock, int offset);

void destroyCodeBlock(CodeBlock* CodeBlock, const char* name);

#endif
