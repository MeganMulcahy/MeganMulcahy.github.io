using System;
using System.Collections.Generic;

namespace CraftingInterpreters.Lisp
{
public class Scanner
{
    private String Source;
    private List<Token> tokens = new List<Token>();
    private int start = 0;
    private int current = 0;
    private int line = 1;

    public Scanner(String source)
    {
        Source = source;
    }

    public List<Token> scanTokens()
    {
        while (!isAtEnd())
        {
            start = current;
            scanToken();
        }
        tokens.Add(new Token(TokenType.EOF, "", null, line));
        return tokens;
    }
    private Boolean isAtEnd()
    {
        return current >= Source.Length;
    }

    private char advance()
    {
        return Source[current++];
    }

    private void addToken(TokenType type)
    {
        addToken(type, null);
    }

    private void addToken(TokenType type, Object literal)
    {
        string text = Source.Substring(start, current - start);
        tokens.Add(new Token(type, text, literal, line));
    }

    private void addString()
    {
        while (peek() != '"' && !isAtEnd())
        {
            advance();
        }

        if (isAtEnd())
        {
            Lisp.error(line, "Unterminated string.");
            return;
        }

        advance();

        // Trim the surrounding quotes and create the string literal.
        string value = Source.Substring(start + 1, current - start - 2);
        addToken(TokenType.STRING, value);
    }
    private void scanToken()
    {
        char c = advance();
        switch (c)
        {
            case '(':
                if (peek() == ')') {
                    advance();
                    addToken(TokenType.NULL);
                } else
                {
                    addToken(TokenType.LEFT_PAREN);
                }
                break;
            case ')': addToken(TokenType.RIGHT_PAREN); break;
            case ';':
                while (peek() != '\n' && !isAtEnd()) advance(); // semicolon is a single-line comment
                break;
            case ' ':
            case '\r':
            case '\t':
                // Ignore whitespace.
                break;
            case '\n':
                line++;
                break;
            default:
                if (Char.IsDigit(c))
                {
                    number();
                }
                else if (c == '"')
                {
                    addString();
                }
                else if (Char.IsLetter(c) || c == '_' || operators(c))
                {
                    symbol();
                }
                break;
        }
    }

    private Boolean operators(char c) {
        switch (c) {
            case '-':
            case '+':
            case '*':
            case '/':
            case '>':
            case '<':
            case '=':
                return true;
            default:
                Lisp.error(line, "Unexpected character.");
                return false;
        }
    }

    private void symbol()
    {
        while (Char.IsLetter(peek()) || Char.IsNumber(peek()) || peek() == '?' || peek() == '_') advance();
        string text = Source.Substring(start, current - start);
        addToken(TokenType.IDENTIFIER);
    }

    private void number()
    {
        while (Char.IsDigit(peek())) advance();

        addToken(TokenType.NUMBER, int.Parse(Source.Substring(start, current - start)));
    }

    private char peek()
    {
        if (isAtEnd()) return '\0';
        return Source[current];
    }
}
}