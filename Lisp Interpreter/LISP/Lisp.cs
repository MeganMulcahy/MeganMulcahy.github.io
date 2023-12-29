using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

namespace CraftingInterpreters.Lisp
{  
public class Lisp
{
    static Boolean hadError = false;
    static Boolean hadRuntimeError = false;

    public static void Main(String[] args)
    {
        if (args.Length > 1)
        {
            Console.WriteLine("Usage: [must add valid file name]");
            Environment.Exit(64);
        }
        else if (args.Length == 0)
        {
            runPrompt();
        }
        else
        {
            runFile(args[0]);
        }
    }

    private static void runFile(String path)
    {
        byte[] bytes = File.ReadAllBytes(path);
        run(Encoding.Default.GetString(bytes));
        if (hadError) Environment.Exit(65);
        if (hadRuntimeError) Environment.Exit(70);
    }

    private static void runPrompt()
    {
        TextReader input = Console.In;
        TextReader reader = input;
        for (;;)
        {
            Console.Write("> ");
            String line = reader.ReadLine();
            if (line == null) break;
            run(line);
            hadError = false;
        }
    }

    private static void run(String source)
    {
        if (source == "EXIT" || source == "exit")
        {
            Console.WriteLine("Exiting...");
            System.Environment.Exit(0);
        }

        Scanner scanner = new Scanner(source);
        List<Token> tokens = scanner.scanTokens();

        Parser parser = new Parser(tokens);
        List<SExpr> sexprs = parser.parse();

        if (hadError) return;

        Interpreter interpreter = new Interpreter(sexprs);
        interpreter.interpret();
    }

    public static void error(int line, String message)
    {
        report(line, "", message);
    }

    private static void report(int line, String where, String message)
    {
        Console.Error.WriteLine("[line " + line + "] Error" + where + ": " + message);
        hadError = true;
    }

    public static void error(Token token, String message)
    {
        if (token.Type == TokenType.EOF)
        {
            report(token.Line, " at end", message);
        }
        else
        {
            report(token.Line, " at '" + token.Lexeme + "'", message);
        }
    }

    public static void runtimeError(RuntimeError error)
    {
        Console.Error.WriteLine(error.Message +
        "\n[line " + error.Token.Line + "]");
        hadRuntimeError = true;
    }
}
}
