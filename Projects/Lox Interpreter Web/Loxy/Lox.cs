using System;
using System.IO;
using System.Collections.Generic;
using CraftingInterpreters.Lox;

namespace CraftingInterpreters.Lox
{
    public class Lox
    {
        private static readonly Interpreter interpreter = new Interpreter();
        private static bool hadError = false;
        private static bool hadRuntimeError = false;

        public static void Main(string[] args)
        {
            if (args.Length > 1)
            {
                Console.WriteLine("Usage: jlox [script]");
                // System.Environment.Exit(64);
            }
            else if (args.Length == 1)
            {
                RunFile(args[0]);
            }
            else
            {
                RunPrompt();
            }

            // if (hadError) System.Environment.Exit(65); // Indicate an error in the exit code.
        }

        static void RunPrompt()
        {
            using (StreamReader reader = new StreamReader(Console.OpenStandardInput()))
            {
                for (;;)
                {
                    Console.Write("> ");
                    string line = reader.ReadLine();
                    if (line == null) break;
                    Run(line);
                    hadError = false; // Reset error flag for each prompt run
                }
            }
        }

        static void RunFile(string path)
        {
            try
            {
                byte[] bytes = File.ReadAllBytes(path);
                Run(System.Text.Encoding.Default.GetString(bytes));
            }
            catch (IOException ex)
            {
                Console.WriteLine($"Error: Could not read file '{path}'");
                Console.WriteLine(ex.Message);
                System.Environment.Exit(1);
            }

            // if (hadError) System.Environment.Exit(65); // Indicate an error in the exit code.
            // if (hadRuntimeError) System.Environment.Exit(70); // Indicate a runtime error in the exit code.
        }

        static void Run(string source)
        {
            if (source == "EXIT" || source == "exit")
            {
                Console.WriteLine("Exiting...");
                System.Environment.Exit(0);
            }

            Scanner scanner = new Scanner(source);
            List<Token> tokens = scanner.ScanTokens();
            Parser parser = new Parser(tokens);
            List<Stmt> statements = parser.Parse();
            Lox.interpreter.Interpret(statements); // Use the static interpreter field
            Expr expression = null; // Initialize expression variable

            // Stop if there was a syntax error.
            // if (hadError) { return; }

            if (expression != null)
                {
                    Console.WriteLine(expression.ToString());
                }  
      }

        public static void Error(int line, string message)
        {
            // Console.WriteLine("Erroring in Lox.cs");
            Report(line, "", message);
        }

        private static void Report(int line, string where, string message)
        {
            Console.Error.WriteLine($"[line {line}] Error{where}: {message}");
            // hadError = true;
        }

        public static void Error(Token token, string message)
        {
            if (token.Type == TokenType.EOF)
            {
                Report(token.Line, " at end", message);
            }
            else
            {
                Report(token.Line, " at '" + token.Lexeme + "'", message);
            }
        }

        public static void RuntimeError(RuntimeError error)
        {
            // Console.WriteLine("Erroring at RuntimeError");

            Console.Error.WriteLine(error.Message +$"\n[line {error.Token.Line}]");
            // hadRuntimeError = true;
        }
    }
}
