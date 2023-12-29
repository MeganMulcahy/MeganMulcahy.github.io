//compiler.h
#ifndef clox_compiler_h
#define clox_compiler_h

#include "object.h"
#include "vm.h"

ObjFunction* compile(const char* root);

void markCompilerRoots();

#endif
