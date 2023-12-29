 using System;
using System.Collections.Generic;

namespace CraftingInterpreters.Lisp
{
public class Parser
{
    private class ParseError : Exception { }

    private List<Token> Tokens;
    private int current = 0;

    /*
    ;; LISP GRAMMAR:
    s => sexp
    sexp => atom | list
    atom => symbol | number | string

    symbol => IDENTIFIER
    number => NUMBER
    string => STRING

    list => ( exprs )
    exprs => epsilon | sexp exprs

    ;; S-Expression Grammar:

    s_expr => simple_data_expr | symbol_expr | list_expr | nested_list_expr
    simple_data_expr => NUMBER
    symbol_expr => IDENTIFIER
    list_expr => ( s_exprs )
    nested_list_expr => ( IDENTIFIER s_exprs )
    */


    public Parser(List<Token> tokens)
    {
        Tokens = tokens;
    }

    public List<SExpr> parse()
    {
        List<SExpr> sexprs = new List<SExpr>();
        while (!isAtEnd())
        {
            sexprs.Add(declaration());
        }

        return sexprs;
    }

// LISP GRAMMAR:

    // s => sexp
    private SExpr declaration() {
        return sexpr();
    }

    // sexp => atom | list
    private SExpr sexpr() {
        if (match(TokenType.IDENTIFIER, TokenType.NUMBER, TokenType.STRING))
        {
            return atom();
        }
        
        if (match(TokenType.NULL))
        {
            return nully();
        }

        //if not an atom is has to be a list

        return listexpr();
    }

   // atom => symbol | number | string
    private SExpr atom() {
        Token t = advance();
        return new SExpr.Atom(t);
    }

    // null => ( )
    private SExpr nully() {
        Token t = advance();
        return new SExpr.Null();
    }

    // list_expr => ( s_exprs )
    // listexpr => ( IDENTIFIER ) | ( NUMBER ) | ( listexpr )
    private SExpr listexpr() {
        consume(TokenType.LEFT_PAREN, "Expect '(' to begin listexpr.");

        SExpr left;

        if (match(TokenType.IDENTIFIER, TokenType.NUMBER, TokenType.STRING))
        {
            left = atom();
        }
        else if (match(TokenType.NULL))
        {
            throw error(peek(), "Cannot insert nothings to beginning of listexpr.");
        }
        else
        {
            // nested loop expr
            left = listexpr();
        }

        List<SExpr> sexprs = exprs();
        consume(TokenType.RIGHT_PAREN, "Expect ')' to end listexpr.");

        return new SExpr.ListExpr(left, sexprs);
    }

    // exprs => epsilon | sexp exprs
    private List<SExpr> exprs() {
        List<SExpr> sexprs = new List<SExpr>();

        while (!match(TokenType.RIGHT_PAREN))
        {
            sexprs.Add(sexpr());
        }

        return sexprs;
    }

// S-Expression Grammar:
    // s_expr => simple_data_expr | symbol_expr | list_expr | nested_list_expr implemented in LISP sexpr() method
    // simple_data_expr => NUMBER in atom() method
    // symbol_expr => IDENTIFIER in atom() method
    // list_expr => ( s_exprs ) in listexpr() method
    // nested_list_expr => ( IDENTIFIER s_exprs ) in listexpr() method

    private Boolean match(params TokenType[] types) 
    {
        foreach (TokenType type in types)
        {
            if (check(type))
            {
                //advance is build in bc its a list or atom?
                return true;
            }
        }

        return false;
    }

    private Token consume(TokenType type, String message)
    {
        if (check(type)) return advance();
        return null;
        //REMOVING THIS LINE SO I DONT ERROR OUT EVERYTIME I DONT PUT A SEMICOLON >:(
        // throw error(peek(), message);
    }

    private Boolean check(TokenType type)
    {
        // Console.WriteLine("Is at end check?");

        if (isAtEnd()) return false;
        return peek().Type == type;
    }

    private Token advance()
    {
        if (!isAtEnd()) current++;
        return previous();
    }

    private Boolean isAtEnd()
    {
        return peek().Type == TokenType.EOF;
    }

    private Token peek()
    {
        return Tokens[current];
    }

    private Token previous()
    {
        return Tokens[current - 1];
    }

    private ParseError error(Token token, String message)
    {
        Lisp.error(token, message);
        return new ParseError();
    }
}
    
}