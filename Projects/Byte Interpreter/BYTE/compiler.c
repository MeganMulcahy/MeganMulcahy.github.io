//compiler.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "compiler.h"
#include "memory.h"
#include "scanner.h"

#ifdef DEBUG_PRINT_CODE
#include "debug.h"
#endif

typedef struct {
  Token parserCurrent;
  Token parserPrev;
  bool parserHadError;
  bool parserPanicMode;
} Parser;

typedef enum {
  PREC_NONE,
  PREC_ASSIGNMENT,  // =
  PREC_OR,          // or
  PREC_AND,         // and
  PREC_EQUALITY,    // == !=
  PREC_COMPARISON,  // < > <= >=
  PREC_TERM,        // + -
  PREC_FACTOR,      // * /
  PREC_UNARY,       // ! -
  PREC_CALL,        // . ()
  PREC_PRIMARY
} Precedence;

typedef void (*ParseFn)(bool canAssign);

typedef struct {
  ParseFn prefix;
  ParseFn infix;
  Precedence precedence;
} ParseRule;

typedef struct {
  Token name;
  int depth;
  bool isCaptured;
} Local;

typedef struct {
  uint8_t index;
  bool isLocal;
} Upvalue;

typedef enum {
  TYPE_FUNCTION,
  TYPE_INITIALIZER,
  TYPE_METHOD,
  TYPE_SCRIPT
} FunctionType;

typedef struct Compiler {
  struct Compiler* enclosing;
  ObjFunction* function;
  FunctionType type;
  Local locals[UINT8_COUNT];
  int localCount;
  Upvalue upvalues[UINT8_COUNT];
  int scopeDepth;
} Compiler;

typedef struct ClassCompiler {
  struct ClassCompiler* enclosing;
  bool hasSuperclass;
} ClassCompiler;

Parser parser;
Compiler* current = NULL;
ClassCompiler* currentClass = NULL;

static CodeBlock* currentCodeBlock() {
  return &current->function->CodeBlock;
}

static void errorAt(Token* token, const char* message) {
  if (parser.parserPanicMode) return;
  parser.parserPanicMode = true;
  fprintf(stderr, "[tokenLine %d] Error", token->tokenLine);

  if (token->type == TOKEN_EOF) {
    fprintf(stderr, " at end");
  } else if (token->type == TOKEN_ERROR) {
  } else {
    fprintf(stderr, " at '%.*s'", token->tokenLength, token->tokenStart);
  }

  fprintf(stderr, ": %s\n", message);
  parser.parserHadError = true;
}

static void error(const char* message) {
  errorAt(&parser.parserPrev, message);
}

static void errorHere(const char* message) {
  errorAt(&parser.parserCurrent, message);
}

void advanceToNextToken() {
  parser.parserPrev = parser.parserCurrent;

  for (;;) {
    parser.parserCurrent = scanToken();
    if (parser.parserCurrent.type != TOKEN_ERROR) break;

    errorHere(parser.parserCurrent.tokenStart);
  }
}

void consume(TokenType type, const char* message) {
  if (parser.parserCurrent.type == type) {
    advanceToNextToken();
    return;
  }

  errorHere(message);
}

bool checkToken(TokenType type) {
  return parser.parserCurrent.type == type;
}

bool matchToken(TokenType type) {
  if (!checkToken(type)) return false;
  advanceToNextToken();
  return true;
}

void sendByte(uint8_t byte) {
  writeToCodeBlock(currentCodeBlock(), byte, parser.parserPrev.tokenLine);
}

void sendBytes(uint8_t byte1, uint8_t byte2) {
  sendByte(byte1);
  sendByte(byte2);
}

void sendLoop(int loopStart) {
  sendByte(LOOP);

  int offset = currentCodeBlock()->codeBlockCount - loopStart + 2;
  if (offset > UINT16_MAX) error("Loop body too large.");

  sendByte((offset >> 8) & 0xff);
  sendByte(offset & 0xff);
}

int sendJump(uint8_t instruction) {
  sendByte(instruction);
  sendByte(0xff);
  sendByte(0xff);
  return currentCodeBlock()->codeBlockCount - 2;
}

void sendReturn() {
  if (current->type == TYPE_INITIALIZER) {
    sendBytes(GET_LOCAL, 0);
  } else {
    sendByte(NIL);
  }

  sendByte(RETURN);
}

static uint8_t makeConstant(Value entryValue) {
  int constant = appendLiteral(currentCodeBlock(), entryValue);
  if (constant > 1000000) {
    error("Too many constants in one CodeBlock.");
    return 0;
  }

  return (uint8_t)constant;
}

void sendConstant(Value value) {
  sendBytes(CONSTANT, makeConstant(value));
}

void patchJump(int offset) {
  int jump = currentCodeBlock()->codeBlockCount - offset - 2;

  if (jump > UINT16_MAX) {
    error("Too much code to jump over.");
  }

  currentCodeBlock()->byteCode[offset] = (jump >> 8) & 0xff;
  currentCodeBlock()->byteCode[offset + 1] = jump & 0xff;
}

void createCompiler(Compiler* compiler, FunctionType type) {
  compiler->enclosing = current;
  compiler->function = NULL;
  compiler->type = type;
  compiler->localCount = 0;
  compiler->scopeDepth = 0;
  compiler->function = createFunction();
  current = compiler;
  if (type != TYPE_SCRIPT) {
    current->function->name = createString(parser.parserPrev.tokenStart, parser.parserPrev.tokenLength);
  }

  Local* local = &current->locals[current->localCount++];
  local->depth = 0;
  local->isCaptured = false;

  if (type != TYPE_FUNCTION) {
    local->name.tokenStart = "this";
    local->name.tokenLength = 4;
  } else {
    local->name.tokenStart = "";
    local->name.tokenLength = 0;
  }
}

ObjFunction* finishCompilation() {
  sendReturn();
  ObjFunction* function = current->function;
#ifdef DEBUG_PRINT_CODE
  if (!parser.parserHadError) {
    destroyCodeBlock(currentCodeBlock(), function->name != NULL
        ? function->name->chars : "<script>");
  }
#endif

  current = current->enclosing;
  return function;
}

void beginScope() {
  current->scopeDepth++;
}

void endScope() {
  current->scopeDepth--;

  while (current->localCount > 0 &&
         current->locals[current->localCount - 1].depth >
            current->scopeDepth) {
    if (current->locals[current->localCount - 1].isCaptured) {
      sendByte(CLOSE_UPVALUE);
    } else {
      sendByte(POP);
    }
    current->localCount--;
  }
}

void expression();
void statement();
void declaration();

ParseRule* getRule(TokenType type);
void parsePrecedence(Precedence precedence);

uint8_t identifierConstant(Token* name) {
  return makeConstant(OBJ_TYPE_VAL(createString(name->tokenStart, name->tokenLength)));
}

bool compareTokens(Token* first, Token* second) {
  if (first->tokenLength != second->tokenLength) return false;
  return memcmp(first->tokenStart, second->tokenStart, first->tokenLength) == 0;
}

int resolveLocal(Compiler* compiler, Token* name) {
  int i = compiler->localCount - 1;
  while (i >= 0) {
      Local* local = &compiler->locals[i];
      if (compareTokens(name, &local->name)) {
          if (local->depth == -1) {
              error("Can't read local variable in its own initializer.");
          }
          return i;
      }
      i--;
  }

  return -1;
}

int addUpvalue(Compiler* compiler, uint8_t index, bool isLocal) {
  int upvalueCount = compiler->function->upvalueCount;
  int i = 0;
  while(i < upvalueCount) {
    Upvalue* upvalue = &compiler->upvalues[i];
    if (upvalue->index == index && upvalue->isLocal == isLocal) {
      return i;
    }
    i++;
  }

  if (upvalueCount == UINT8_COUNT) {
    error("Too many closure variables in function.");
    return 0;
  }

  compiler->upvalues[upvalueCount].isLocal = isLocal;
  compiler->upvalues[upvalueCount].index = index;
  return compiler->function->upvalueCount++;
}

int resolveUpvalue(Compiler* compiler, Token* name) {
  if (compiler->enclosing == NULL) return -1;

  int local = resolveLocal(compiler->enclosing, name);
  if (local != -1) {
    compiler->enclosing->locals[local].isCaptured = true;
    return addUpvalue(compiler, (uint8_t)local, true);
  }

  int upvalue = resolveUpvalue(compiler->enclosing, name);
  if (upvalue != -1) {
    return addUpvalue(compiler, (uint8_t)upvalue, false);
  }

  return -1;
}

void addLocal(Token name) {
  if (current->localCount == UINT8_COUNT) {
    error("Too many local variables in function.");
    return;
  }
  Local* local = &current->locals[current->localCount++];
  local->name = name;
  local->depth = -1;
  local->isCaptured = false;
}

void declareVariable() {
  if (current->scopeDepth == 0) return;

  Token* name = &parser.parserPrev;
  int i = current->localCount - 1;
  while (i >= 0) {
      Local* local = &current->locals[i];

      if (local->depth != -1 && local->depth < current->scopeDepth) {
          break;
      }

      if (compareTokens(name, &local->name)) {
          error("Already a variable with this name in this scope.");
      }
      i--;
  }

  addLocal(*name);
}

uint8_t parseVariable(const char* errorMessage) {
  consume(TOKEN_IDENTIFIER, errorMessage);
  declareVariable();
  if (current->scopeDepth > 0) return 0;
  return identifierConstant(&parser.parserPrev);
}

void markVariableAsInitialized() {
  if (current->scopeDepth == 0) return;
  current->locals[current->localCount - 1].depth =
      current->scopeDepth;
}

void defineVariable(uint8_t global) {
  if (current->scopeDepth > 0) {
    markVariableAsInitialized();
    return;
  }

  sendBytes(DEFINE_GLOBAL, global);
}

uint8_t argumentList() {
  uint8_t argCount = 0;
  if (!checkToken(TOKEN_RIGHT_PAREN)) {
    do {
      expression();
      if (argCount == 255) {
        error("Can't have more than 255 arguments.");
      }
      argCount++;
    } while (matchToken(TOKEN_COMMA));
  }
  consume(TOKEN_RIGHT_PAREN, "Expect ')' after arguments.");
  return argCount;
}

void and_(bool canAssign) {
  int endJump = sendJump(JUMP_IF_FALSE);

  sendByte(POP);
  parsePrecedence(PREC_AND);

  patchJump(endJump);
}

void binary(bool canAssign) {
  TokenType operatorType = parser.parserPrev.type;
  ParseRule* rule = getRule(operatorType);
  parsePrecedence((Precedence)(rule->precedence + 1));

  if (operatorType == TOKEN_BANG_EQUAL) {
    sendBytes(EQUAL, NOT);
  }
  else if (operatorType == TOKEN_EQUAL_EQUAL) {
    sendByte(EQUAL);
  } 
  else if (operatorType == TOKEN_GREATER) {
    sendByte(GREATER);
  } 
  else if (operatorType == TOKEN_GREATER_EQUAL) {
    sendBytes(LESS, NOT);
  } 
  else if (operatorType == TOKEN_LESS) {
    sendByte(LESS);
  } 
  else if (operatorType == TOKEN_LESS_EQUAL) {
    sendBytes(GREATER, NOT);
  } 
  else if (operatorType == TOKEN_PLUS) {
    sendByte(ADD);
  }
  else if (operatorType == TOKEN_MINUS) {
    sendByte(SUBTRACT);
  } 
  else if (operatorType == TOKEN_STAR) {
    sendByte(MULTIPLY);
  } 
  else if (operatorType == TOKEN_SLASH) {
    sendByte(DIVIDE);
  } 
  else {
    return;
}

}

void call(bool canAssign) {
  uint8_t argCount = argumentList();
  sendBytes(CALL, argCount);
}

void dot(bool canAssign) {
  consume(TOKEN_IDENTIFIER, "Expect property name after '.'.");
  uint8_t name = identifierConstant(&parser.parserPrev);

  if (canAssign && matchToken(TOKEN_EQUAL)) {
    expression();
    sendBytes(SET_PROPERTY, name);
  } else if (matchToken(TOKEN_LEFT_PAREN)) {
    uint8_t argCount = argumentList();
    sendBytes(INVOKE, name);
    sendByte(argCount);
  } else {
    sendBytes(GET_PROPERTY, name);
  }
}

void literal(bool canAssign) {
  if (parser.parserPrev.type == TOKEN_FALSE) {
    sendByte(FALSE);
  }
  else if (parser.parserPrev.type == TOKEN_NIL) {
    sendByte(NIL);
  }
  else if (parser.parserPrev.type == TOKEN_TRUE) {
    sendByte(TRUE);
  }
  else {
    return;
}

}

void grouping(bool canAssign) {
  expression();
  consume(TOKEN_RIGHT_PAREN, "Expect ')' after expression.");
}

void number(bool canAssign) {
  double value = strtod(parser.parserPrev.tokenStart, NULL);
  sendConstant(NUMBER_VAL(value));
}

void or_(bool canAssign) {
  int elseJump = sendJump(JUMP_IF_FALSE);
  int endJump = sendJump(JUMP);

  patchJump(elseJump);
  sendByte(POP);

  parsePrecedence(PREC_OR);
  patchJump(endJump);
}

static void string(bool canAssign) {
  sendConstant(OBJ_TYPE_VAL(createString(parser.parserPrev.tokenStart + 1, parser.parserPrev.tokenLength - 2)));
}

void namedVariable(Token name, bool canAssign) {
  uint8_t getOp, setOp;
  int arg = resolveLocal(current, &name);
  if (arg != -1) {
    getOp = GET_LOCAL;
    setOp = SET_LOCAL;
  } else if ((arg = resolveUpvalue(current, &name)) != -1) {
    getOp = GET_UPVALUE;
    setOp = SET_UPVALUE;
  } else {
    arg = identifierConstant(&name);
    getOp = GET_GLOBAL;
    setOp = SET_GLOBAL;
  }
  if (canAssign && matchToken(TOKEN_EQUAL)) {
    expression();
    sendBytes(setOp, (uint8_t)arg);
  } else {
    sendBytes(getOp, (uint8_t)arg);
  }
}

void variable(bool canAssign) {
  namedVariable(parser.parserPrev, canAssign);
}

Token syntheticToken(const char* text) {
  Token token;
  token.tokenStart = text;
  token.tokenLength = (int)strlen(text);
  return token;
}

void super_(bool canAssign) {
  if (currentClass == NULL) {
    error("Can't use 'super' outside of a class.");
  } else if (!currentClass->hasSuperclass) {
    error("Can't use 'super' in a class with no superclass.");
  }

  consume(TOKEN_DOT, "Expect '.' after 'super'.");
  consume(TOKEN_IDENTIFIER, "Expect superclass method name.");
  uint8_t name = identifierConstant(&parser.parserPrev);
  
  namedVariable(syntheticToken("this"), false);
  if (matchToken(TOKEN_LEFT_PAREN)) {
    uint8_t argCount = argumentList();
    namedVariable(syntheticToken("super"), false);
    sendBytes(SUPER_INVOKE, name);
    sendByte(argCount);
  } else {
    namedVariable(syntheticToken("super"), false);
    sendBytes(GET_SUPER, name);
  }
}

void this_(bool canAssign) {
  if (currentClass == NULL) {
    error("Can't use 'this' outside of a class.");
    return;
  }
  variable(false);
} 

void unary(bool canAssign) {
  TokenType operatorType = parser.parserPrev.type;

  parsePrecedence(PREC_UNARY);
  
  if (operatorType == TOKEN_BANG) {
    sendByte(NOT);
  } 
  else if (operatorType == TOKEN_MINUS) {
    sendByte(NEGATE);
  }
  else {
    return;
  }

}

ParseRule rules[] = {
  [TOKEN_LEFT_PAREN]    = {grouping, call,   PREC_CALL},
  [TOKEN_RIGHT_PAREN]   = {NULL,     NULL,   PREC_NONE},
  [TOKEN_LEFT_BRACE]    = {NULL,     NULL,   PREC_NONE},
  [TOKEN_RIGHT_BRACE]   = {NULL,     NULL,   PREC_NONE},
  [TOKEN_COMMA]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_DOT]           = {NULL,     dot,    PREC_CALL},
  [TOKEN_MINUS]         = {unary,    binary, PREC_TERM},
  [TOKEN_PLUS]          = {NULL,     binary, PREC_TERM},
  [TOKEN_SEMICOLON]     = {NULL,     NULL,   PREC_NONE},
  [TOKEN_SLASH]         = {NULL,     binary, PREC_FACTOR},
  [TOKEN_STAR]          = {NULL,     binary, PREC_FACTOR},
  [TOKEN_BANG]          = {unary,    NULL,   PREC_NONE},
  [TOKEN_BANG_EQUAL]    = {NULL,     binary, PREC_EQUALITY},
  [TOKEN_EQUAL]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_EQUAL_EQUAL]   = {NULL,     binary, PREC_EQUALITY},
  [TOKEN_GREATER]       = {NULL,     binary, PREC_COMPARISON},
  [TOKEN_GREATER_EQUAL] = {NULL,     binary, PREC_COMPARISON},
  [TOKEN_LESS]          = {NULL,     binary, PREC_COMPARISON},
  [TOKEN_LESS_EQUAL]    = {NULL,     binary, PREC_COMPARISON},
  [TOKEN_IDENTIFIER]    = {variable, NULL,   PREC_NONE},
  [TOKEN_STRING]        = {string,   NULL,   PREC_NONE},
  [TOKEN_NUMBER]        = {number,   NULL,   PREC_NONE},
  [TOKEN_AND]           = {NULL,     and_,   PREC_AND},
  [TOKEN_CLASS]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_ELSE]          = {NULL,     NULL,   PREC_NONE},
  [TOKEN_FALSE]         = {literal,  NULL,   PREC_NONE},
  [TOKEN_FOR]           = {NULL,     NULL,   PREC_NONE},
  [TOKEN_FUN]           = {NULL,     NULL,   PREC_NONE},
  [TOKEN_IF]            = {NULL,     NULL,   PREC_NONE},
  [TOKEN_NIL]           = {literal,  NULL,   PREC_NONE},
  [TOKEN_OR]            = {NULL,     or_,    PREC_OR},
  [TOKEN_PRINT]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_RETURN]        = {NULL,     NULL,   PREC_NONE},
  [TOKEN_SUPER]         = {super_,   NULL,   PREC_NONE},
  [TOKEN_THIS]          = {this_,    NULL,   PREC_NONE},
  [TOKEN_TRUE]          = {literal,  NULL,   PREC_NONE},
  [TOKEN_VAR]           = {NULL,     NULL,   PREC_NONE},
  [TOKEN_WHILE]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_ERROR]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_EOF]           = {NULL,     NULL,   PREC_NONE},
};

void parsePrecedence(Precedence precedence) {
  advanceToNextToken();
  ParseFn prefixRule = getRule(parser.parserPrev.type)->prefix;
  if (prefixRule == NULL) {
    error("Expect expression.");
    return;
  }
  bool canAssign = precedence <= PREC_ASSIGNMENT;
  prefixRule(canAssign);
  while (precedence <= getRule(parser.parserCurrent.type)->precedence) {
    advanceToNextToken();
    ParseFn infixRule = getRule(parser.parserPrev.type)->infix;
    infixRule(canAssign);  
  }

  if (canAssign && matchToken(TOKEN_EQUAL)) {
    error("Invalid assignment target.");
  }
}

ParseRule* getRule(TokenType type) {
  return &rules[type];
}

void expression() {
  parsePrecedence(PREC_ASSIGNMENT);
}

void block() {
  while (!checkToken(TOKEN_RIGHT_BRACE) && !checkToken(TOKEN_EOF)) {
    declaration();
  }

  consume(TOKEN_RIGHT_BRACE, "Expect '}' after block.");
}

void function(FunctionType type) {
  Compiler compiler;
  createCompiler(&compiler, type);
  beginScope();

  consume(TOKEN_LEFT_PAREN, "Expect '(' after function name.");
  if (!checkToken(TOKEN_RIGHT_PAREN)) {
    do {
      current->function->arity++;
      if (current->function->arity > 255) {
        errorHere("Can't have more than 255 parameters.");
      }
      uint8_t constant = parseVariable("Expect parameter name.");
      defineVariable(constant);
    } while (matchToken(TOKEN_COMMA));
  }
  consume(TOKEN_RIGHT_PAREN, "Expect ')' after parameters.");
  consume(TOKEN_LEFT_BRACE, "Expect '{' before function body.");
  block();

  ObjFunction* function = finishCompilation();

  sendBytes(CLOSURE, makeConstant(OBJ_TYPE_VAL(function)));

  for (int i = 0; i < function->upvalueCount; i++) {
    sendByte(compiler.upvalues[i].isLocal ? 1 : 0);
    sendByte(compiler.upvalues[i].index);
  }
}

void method() {
  consume(TOKEN_IDENTIFIER, "Expect method name.");
  uint8_t constant = identifierConstant(&parser.parserPrev);
  FunctionType type = TYPE_METHOD;
  if (parser.parserPrev.tokenLength == 4 &&
      memcmp(parser.parserPrev.tokenStart, "init", 4) == 0) {
    type = TYPE_INITIALIZER;
  }
  function(type);
  sendBytes(METHOD, constant);
}

void classDeclaration() {
  consume(TOKEN_IDENTIFIER, "Expect class name.");
  Token className = parser.parserPrev;
  uint8_t nameConstant = identifierConstant(&parser.parserPrev);
  declareVariable();

  sendBytes(CLASS, nameConstant);
  defineVariable(nameConstant);

  ClassCompiler classCompiler;
  classCompiler.hasSuperclass = false;
  classCompiler.enclosing = currentClass;
  currentClass = &classCompiler;

  if (matchToken(TOKEN_LESS)) {
    consume(TOKEN_IDENTIFIER, "Expect superclass name.");
    variable(false);
  if (compareTokens(&className, &parser.parserPrev)) {
      error("A class can't inherit from itself.");
    }
  beginScope();
  addLocal(syntheticToken("super"));
  defineVariable(0);
  namedVariable(className, false);
  sendByte(INHERIT);
  classCompiler.hasSuperclass = true;
  }

  namedVariable(className, false);
  consume(TOKEN_LEFT_BRACE, "Expect '{' before class body.");
  while (!checkToken(TOKEN_RIGHT_BRACE) && !checkToken(TOKEN_EOF)) {
    method();
  }
  consume(TOKEN_RIGHT_BRACE, "Expect '}' after class body.");
  sendByte(POP);
  if (classCompiler.hasSuperclass) {
    endScope();
  }
  currentClass = currentClass->enclosing;
}

void funDeclaration() {
  uint8_t global = parseVariable("Expect function name.");
  markVariableAsInitialized();
  function(TYPE_FUNCTION);
  defineVariable(global);
}

void varDeclaration() {
  uint8_t global = parseVariable("Expect variable name.");

  if (matchToken(TOKEN_EQUAL)) {
    expression();
  } else {
    sendByte(NIL);
  }
  consume(TOKEN_SEMICOLON,
         "Expect ';' after variable declaration.");

  defineVariable(global);
}

void expressionStatement() {
  expression();
  consume(TOKEN_SEMICOLON, "Expect ';' after expression.");
  sendByte(POP);
}

void forStatement() {
  beginScope();
  consume(TOKEN_LEFT_PAREN, "Expect '(' after 'for'.");
  if (matchToken(TOKEN_SEMICOLON)) {
  } else if (matchToken(TOKEN_VAR)) {
    varDeclaration();
  } else {
    expressionStatement();
  }
  int loopStart = currentCodeBlock()->codeBlockCount;
  int exitJump = -1;
  if (!matchToken(TOKEN_SEMICOLON)) {
    expression();
    consume(TOKEN_SEMICOLON, "Expect ';' after loop condition.");
    exitJump = sendJump(JUMP_IF_FALSE);
    sendByte(POP); 
  }

  if (!matchToken(TOKEN_RIGHT_PAREN)) {
    int bodyJump = sendJump(JUMP);
    int incrementStart = currentCodeBlock()->codeBlockCount;
    expression();
    sendByte(POP);
    consume(TOKEN_RIGHT_PAREN, "Expect ')' after for clauses.");

    sendLoop(loopStart);
    loopStart = incrementStart;
    patchJump(bodyJump);
  }

  statement();
  sendLoop(loopStart);

  if (exitJump != -1) {
    patchJump(exitJump);
    sendByte(POP);
  }

  endScope();
}

void ifStatement() {
  consume(TOKEN_LEFT_PAREN, "Expect '(' after 'if'.");
  expression();
  consume(TOKEN_RIGHT_PAREN, "Expect ')' after condition."); // [paren]
  int thenJump = sendJump(JUMP_IF_FALSE);
  sendByte(POP);
  statement();
  int elseJump = sendJump(JUMP);
  patchJump(thenJump);
  sendByte(POP);
  if (matchToken(TOKEN_ELSE)) statement();
  patchJump(elseJump);
}

void printStatement() {
  expression();
  consume(TOKEN_SEMICOLON, "Expect ';' after value.");
  sendByte(PRINT);
}

void returnStatement() {
  if (current->type == TYPE_SCRIPT) {
    error("Can't return from top-level code.");
  }
  if (matchToken(TOKEN_SEMICOLON)) {
    sendReturn();
  } else {
    if (current->type == TYPE_INITIALIZER) {
      error("Can't return a value from an initializer.");
    }
    expression();
    consume(TOKEN_SEMICOLON, "Expect ';' after return value.");
    sendByte(RETURN);
  }
}

void whileStatement() {
  int loopStart = currentCodeBlock()->codeBlockCount;
  consume(TOKEN_LEFT_PAREN, "Expect '(' after 'while'.");
  expression();
  consume(TOKEN_RIGHT_PAREN, "Expect ')' after condition.");
  int exitJump = sendJump(JUMP_IF_FALSE);
  sendByte(POP);
  statement();
  sendLoop(loopStart);
  patchJump(exitJump);
  sendByte(POP);
}

void synchronize() {
  parser.parserPanicMode = false;

  while (parser.parserCurrent.type != TOKEN_EOF) {
    if (parser.parserPrev.type == TOKEN_SEMICOLON) return;
    if (parser.parserCurrent.type == TOKEN_CLASS || parser.parserCurrent.type == TOKEN_FUN || parser.parserCurrent.type == TOKEN_VAR || parser.parserCurrent.type == TOKEN_FOR || parser.parserCurrent.type == TOKEN_IF || parser.parserCurrent.type == TOKEN_WHILE || parser.parserCurrent.type == TOKEN_PRINT || parser.parserCurrent.type == TOKEN_RETURN) {
    return;
  } else {
  }
    advanceToNextToken();
  }
}

void declaration() {
  if (matchToken(TOKEN_CLASS)) {
    classDeclaration();
  } else if (matchToken(TOKEN_FUN)) {
    funDeclaration();
  } else if (matchToken(TOKEN_VAR)) {
    varDeclaration();
  } else {
    statement();
  }
  if (parser.parserPanicMode) synchronize();
}

void statement() {
  if (matchToken(TOKEN_PRINT)) {
    printStatement();
  } else if (matchToken(TOKEN_FOR)) {
    forStatement();
  } else if (matchToken(TOKEN_IF)) {
    ifStatement();
  } else if (matchToken(TOKEN_RETURN)) {
    returnStatement();
  } else if (matchToken(TOKEN_WHILE)) {
    whileStatement();
  } else if (matchToken(TOKEN_LEFT_BRACE)) {
    beginScope();
    block();
    endScope();
  } else {
    expressionStatement();
  }
}

ObjFunction* compile(const char* root) {
  createScanner(root);
  Compiler compiler;
  createCompiler(&compiler, TYPE_SCRIPT);
  parser.parserHadError = false;
  parser.parserPanicMode = false;
  advanceToNextToken();
  while (!matchToken(TOKEN_EOF)) {
    declaration();
  }
  ObjFunction* function = finishCompilation();
  return parser.parserHadError ? NULL : function;
}

void markCompilerRoots() {
  Compiler* compiler = current;
  while (compiler != NULL) {
    indicateObject((Obj*)compiler->function);
    compiler = compiler->enclosing;
  }
}
