//main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"

static void readTerminal() {
  char line[1024];
  for (;;) {
    printf("> ");

    if (!fgets(line, sizeof(line), stdin)) {
      printf("\n");
      break;
    }

    if (strcmp(line, "exit\n") == 0 || strcmp(line, "EXIT\n") == 0 || strcmp(line, "exit;\n") == 0 || strcmp(line, "EXIT;\n") == 0) {
      printf("Exiting...\n");

      break;
    }

    interpret(line);
  }
}

static char* readFile(const char* path) {
  FILE* file = fopen(path, "rb");
  if (file == NULL) {
        fprintf(stderr, "Error: Could not open file \"%s\".\n", path);
        exit(74);
  }

  fseek(file, 0L, SEEK_END);
  size_t fileSize = ftell(file);
  rewind(file);

  char* sourceCode = (char*)malloc(fileSize + 1);
  if (sourceCode == NULL) {
      fprintf(stderr, "Error: Not enough memory to read \"%s\".\n", path);
      exit(74);
  }

  size_t bytesRead = fread(sourceCode, sizeof(char), fileSize, file);
  if (bytesRead < fileSize) {
      fprintf(stderr, "Error: Could not read file \"%s\".\n", path);
      exit(74);
  }
  
  sourceCode[bytesRead] = '\0';

  fclose(file);
  return sourceCode;
}

static void runFile(const char* path) {
  char* root = readFile(path);
  Interpreter result = interpret(root);
  free(root);

  if (result == INTERPRET_COMPILE_ERROR) exit(65);
  if (result == INTERPRET_RUNTIME_ERROR) exit(70);
}

int main(int argc, const char* argv[]) {
  createProcessor();
  if (argc == 1) {
    readTerminal();
  } 
  else if (argc == 2) {
    runFile(argv[1]);
  } 
  else {
    fprintf(stderr, "Usage: clox [path]\n");
    exit(64);
  }
  
  deleteProcessor();

  return 0;
}
