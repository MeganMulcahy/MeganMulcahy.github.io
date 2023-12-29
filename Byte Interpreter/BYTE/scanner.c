//scanner.c
#include <stdio.h>
#include <string.h>

#include "common.h"
#include "scanner.h"

typedef struct {
  const char* scannerStart;
  const char* scannerCurrentLine;
  int scannerLine;
} Scanner;

Scanner scanner;

void createScanner(const char* root) {
  scanner.scannerStart = root;
  scanner.scannerCurrentLine = root;
  scanner.scannerLine = 1;
}

static bool isAlpha(char inputChar) {
  return (inputChar >= 'a' && inputChar <= 'z') || (inputChar >= 'A' && inputChar <= 'Z') || inputChar == '_';
}

static bool isDigit(char inputChar) {
  return inputChar >= '0' && inputChar <= '9';
}

static bool atEnd() {
  return *scanner.scannerCurrentLine == '\0';
}

static char advanceToNextToken() {
  scanner.scannerCurrentLine++;
  return scanner.scannerCurrentLine[-1];
}

static char peekValue() {
  return *scanner.scannerCurrentLine;
}

static char peekNext() {
  if (atEnd()) return '\0';
  return scanner.scannerCurrentLine[1];
}

static bool matchToken(char expected) {
  if (atEnd()) return false;
  if (*scanner.scannerCurrentLine != expected) return false;
  scanner.scannerCurrentLine++;
  return true;
}

static Token createToken(TokenType type) {
  Token token;
  token.type = type;
  token.tokenStart = scanner.scannerStart;
  token.tokenLength = (int)(scanner.scannerCurrentLine - scanner.scannerStart);
  token.tokenLine = scanner.scannerLine;
  return token;
}

static Token errorToken(const char* message) {
  Token token;
  token.type = TOKEN_ERROR;
  token.tokenStart = message;
  token.tokenLength = (int)strlen(message);
  token.tokenLine = scanner.scannerLine;
  return token;
}

static void ignoreWhiteSpace() {
  for (;;) {
    char inputChars = peekValue();
   if (inputChars == ' ' || inputChars == '\r' || inputChars == '\t') {
    advanceToNextToken();
  } 
  else if (inputChars == '\n') {
      scanner.scannerLine++;
      advanceToNextToken();
  } 
  else if (inputChars == '/') {
      if (peekNext() == '/') {
          while (peekValue() != '\n' && !atEnd()) {
              advanceToNextToken();
          }
      } else {
          return;
      }
  } 
  else {
      return;
  }

  }
}

static TokenType checkKeyword(int scannerStart, int length,
    const char* rest, TokenType type) {
  if (scanner.scannerCurrentLine - scanner.scannerStart == scannerStart + length &&
      memcmp(scanner.scannerStart + scannerStart, rest, length) == 0) {
    return type;
  }

  return TOKEN_IDENTIFIER;
}

static TokenType identifierType() {
 if (scanner.scannerStart[0] == 'a') {
    return checkKeyword(1, 2, "nd", TOKEN_AND);
  } 
  else if (scanner.scannerStart[0] == 'c') {
      return checkKeyword(1, 4, "lass", TOKEN_CLASS);
  } 
  else if (scanner.scannerStart[0] == 'e') {
      return checkKeyword(1, 3, "lse", TOKEN_ELSE);
  } 
  else if (scanner.scannerStart[0] == 'f') {
      if (scanner.scannerCurrentLine - scanner.scannerStart > 1) {
          if (scanner.scannerStart[1] == 'a') {
              return checkKeyword(2, 3, "lse", TOKEN_FALSE);
          } 
          else if (scanner.scannerStart[1] == 'o') {
              return checkKeyword(2, 1, "r", TOKEN_FOR);
          } 
          else if (scanner.scannerStart[1] == 'u') {
              return checkKeyword(2, 1, "n", TOKEN_FUN);
          }
      }
  } 
  else if (scanner.scannerStart[0] == 'i') {
      return checkKeyword(1, 1, "f", TOKEN_IF);
  } 
  else if (scanner.scannerStart[0] == 'n') {
      return checkKeyword(1, 2, "il", TOKEN_NIL);
  } 
  else if (scanner.scannerStart[0] == 'o') {
      return checkKeyword(1, 1, "r", TOKEN_OR);
  } 
  else if (scanner.scannerStart[0] == 'p') {
      return checkKeyword(1, 4, "rint", TOKEN_PRINT);
  } 
  else if (scanner.scannerStart[0] == 'r') {
      return checkKeyword(1, 5, "eturn", TOKEN_RETURN);
  } 
  else if (scanner.scannerStart[0] == 's') {
      return checkKeyword(1, 4, "uper", TOKEN_SUPER);
  } 
  else if (scanner.scannerStart[0] == 't') {
      if (scanner.scannerCurrentLine - scanner.scannerStart > 1) {
          if (scanner.scannerStart[1] == 'h') {
              return checkKeyword(2, 2, "is", TOKEN_THIS);
          } 
          else if (scanner.scannerStart[1] == 'r') {
              return checkKeyword(2, 2, "ue", TOKEN_TRUE);
          }
      }
  } 
  else if (scanner.scannerStart[0] == 'v') {
      return checkKeyword(1, 2, "ar", TOKEN_VAR);
  } 
  else if (scanner.scannerStart[0] == 'w') {
      return checkKeyword(1, 4, "hile", TOKEN_WHILE);
  }


  return TOKEN_IDENTIFIER;
}

static Token identifier() {
  while (isAlpha(peekValue()) || isDigit(peekValue())) advanceToNextToken();
  return createToken(identifierType());
}

static Token number() {
  while (isDigit(peekValue())) advanceToNextToken();

  if (peekValue() == '.' && isDigit(peekNext())) {
    advanceToNextToken();
    while (isDigit(peekValue())) advanceToNextToken();
  }

  return createToken(TOKEN_NUMBER);
}

static Token string() {
  while (peekValue() != '"' && !atEnd()) {
    if (peekValue() == '\n') scanner.scannerLine++;
    advanceToNextToken();
  }

  if (atEnd()) return errorToken("Unterminated string.");

  advanceToNextToken();
  return createToken(TOKEN_STRING);
}

Token scanToken() {
  ignoreWhiteSpace();
  scanner.scannerStart = scanner.scannerCurrentLine;

  if (atEnd()) return createToken(TOKEN_EOF);
  
  char inputChars = advanceToNextToken();
  if (isAlpha(inputChars)) return identifier();
  if (isDigit(inputChars)) return number();

  if (inputChars == '(') {
    return createToken(TOKEN_LEFT_PAREN);
  } 
  else if (inputChars == ')') {
      return createToken(TOKEN_RIGHT_PAREN);
  } 
  else if (inputChars == '{') {
      return createToken(TOKEN_LEFT_BRACE);
  } 
  else if (inputChars == '}') {
      return createToken(TOKEN_RIGHT_BRACE);
  } 
  else if (inputChars == ';') {
      return createToken(TOKEN_SEMICOLON);
  } 
  else if (inputChars == ',') {
      return createToken(TOKEN_COMMA);
  } 
  else if (inputChars == '.') {
      return createToken(TOKEN_DOT);
  } 
  else if (inputChars == '-') {
      return createToken(TOKEN_MINUS);
  } 
  else if (inputChars == '+') {
      return createToken(TOKEN_PLUS);
  } 
  else if (inputChars == '/') {
      return createToken(TOKEN_SLASH);
  } 
  else if (inputChars == '*') {
      return createToken(TOKEN_STAR);
  } 
  else if (inputChars == '!') {
      return createToken(matchToken('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
  } 
  else if (inputChars == '=') {
      return createToken(matchToken('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
  } 
  else if (inputChars == '<') {
      return createToken(matchToken('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
  } 
  else if (inputChars == '>') {
      return createToken(matchToken('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
  } 
  else if (inputChars == '"') {
      return string();
  }
  return errorToken("Unexpected character.");
}