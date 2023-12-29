using System;
using System.Collections.Generic;

namespace CraftingInterpreters.Lox
{
    class Scanner
    {
        private readonly string source;
        private readonly List<Token> tokens = new List<Token>();
        private int start = 0;
        private int current = 0;
        private int line = 1;

        private static readonly Dictionary<string, TokenType> keywords = new Dictionary<string, TokenType>
        {
            { "and", TokenType.AND },
            { "class", TokenType.CLASS },
            { "else", TokenType.ELSE },
            { "false", TokenType.FALSE },
            { "for", TokenType.FOR },
            { "fun", TokenType.FUN },
            { "if", TokenType.IF },
            { "nil", TokenType.NIL },
            { "or", TokenType.OR },
            { "print", TokenType.PRINT },
            { "return", TokenType.RETURN },
            { "super", TokenType.SUPER },
            { "this", TokenType.THIS },
            { "true", TokenType.TRUE },
            { "var", TokenType.VAR },
            { "while", TokenType.WHILE }
        };

        public Scanner(string source)
        {
            this.source = source;
        }

        public List<Token> ScanTokens()
        {
            while (!IsAtEnd())
            {
                start = current;
                ScanToken();
            }

            tokens.Add(new Token(TokenType.EOF, "", null, line));
            return tokens;
        }

        private bool IsAtEnd()
        {
            return current >= source.Length;
        }

        private char Advance()
        {
            return source[current++];
        }

        private void AddToken(TokenType type)
        {
            AddToken(type, null);
        }

        private void AddToken(TokenType type, object literal)
        {
            string text = source.Substring(start, current - start);
            tokens.Add(new Token(type, text, literal, line));
        }

        private char Peek()
        {
            if (IsAtEnd()) return '\0';
            return source[current];
        }

        private char PeekNext()
        {
            if (current + 1 >= source.Length) return '\0';
            return source[current + 1];
        }

        private void ScanToken()
        {
            char c = Advance();
            switch (c)
            {
        // GREATER, GREATER_EQUAL,
        // LESS, LESS_EQUAL,
        // IDENTIFIER, STRING, NUMBER,
        // AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
        // PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,
                case '(': AddToken(TokenType.LEFT_PAREN); break;
                case ')': AddToken(TokenType.RIGHT_PAREN); break;
                case '{': AddToken(TokenType.LEFT_BRACE); break;
                case '}': AddToken(TokenType.RIGHT_BRACE); break;
                case ',': AddToken(TokenType.COMMA); break;
                case '.': AddToken(TokenType.DOT); break;
                // case '=': AddToken(TokenType.EQUAL); break;
                case '-': AddToken(TokenType.MINUS); break;
                // case '<': AddToken(TokenType.LESS); break;
                case '<':
                    AddToken(Match('=') ? TokenType.LESS_EQUAL : TokenType.LESS);
                    break;
                case '>':
                    AddToken(Match('=') ? TokenType.GREATER_EQUAL : TokenType.GREATER);
                    break;
                case '=':
                    AddToken(Match('=') ? TokenType.EQUAL_EQUAL : TokenType.EQUAL);
                    break;
                case '!':
                    AddToken(Match('=') ? TokenType.BANG_EQUAL : TokenType.BANG);
                    break;
                // case "<=": AddToken(TokenType.LESS_EQUAL); break;
                // case '>': AddToken(TokenType.GREATER); break;
                // case ">=": AddToken(TokenType.GREATER_EQUAL); break;
                // case "==": AddToken(TokenType.EQUAL_EQUAL); break;
                case '+': AddToken(TokenType.PLUS); break;
                case ';': AddToken(TokenType.SEMICOLON); break;
                // case '!': AddToken(TokenType.BANG); break;
                // case "!=": AddToken(TokenType.BANG_EQUAL); break;
                case '*': AddToken(TokenType.STAR); break;
                case '/':
                    if (Match('/'))
                    {
                        // A comment goes until the end of the line.
                        while (Peek() != '\n' && !IsAtEnd()) Advance();
                    }
                    else
                    {
                        AddToken(TokenType.SLASH);
                    }
                    break;
                case ' ':
                case '\r':
                case '\t':
                    // Ignore whitespace.
                    break;
                case '\n':
                    line++;
                    break;
                case '"': String(); break;
                default:
                    if (IsDigit(c))
                    {
                        // Console.WriteLine("IsDigit");
                        Number();
                    }
                    else if (IsAlpha(c))
                    {
                        // Console.WriteLine("isAlpha");
                        Identifier();
                    }
                    else
                    {
                        Console.WriteLine("erroring in Scanner.cs Lox.error");

                        Lox.Error(line, "Unexpected character.");
                    }
                    break;
            }
        }

        private void String()
{
    while (Peek() != '"' && !IsAtEnd() && Peek() != '=')
    {
        if (Peek() == '\n') line++;
        Advance();
    }

    if (IsAtEnd() || Peek() == '=')
    {
        Lox.Error(line, "Unterminated string or unexpected equal sign.");
        return;
    }

    // The closing ".
    Advance();

    // Trim the surrounding quotes.
    string value = source.Substring(start + 1, current - start - 2);
    AddToken(TokenType.STRING, value);
}


        private void Identifier()
        {
            while (IsAlphaNumeric(Peek())) Advance();

            string text = source.Substring(start, current - start);
            var keyExists = keywords.ContainsKey(text);
            TokenType type = keywords.TryGetValue(text, out TokenType keywordType) ? keywordType : TokenType.IDENTIFIER;
            AddToken(type);
        }

        private void Number()
        {
            while (IsDigit(Peek())) Advance();

            // Look for a fractional part.
            if (Peek() == '.' && IsDigit(PeekNext()))
            {
                // Consume the "."
                Advance();

                while (IsDigit(Peek())) Advance();
            }

            AddToken(TokenType.NUMBER, double.Parse(source.Substring(start, current - start)));
        }

        private bool IsAlpha(char c)
        {
            return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
        }

        private bool IsAlphaNumeric(char c)
        {
            return IsAlpha(c) || IsDigit(c);
        }

        private bool IsDigit(char c)
        {
            return c >= '0' && c <= '9';
        }

        private bool Match(char expected)
        {
            // Console.WriteLine("Match function");
            if (IsAtEnd()) return false;
            if (source[current] != expected) return false;

            current++;
            return true;
        }
    }
}
