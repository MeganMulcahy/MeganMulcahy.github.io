using System;
using System.Collections.Generic;

namespace CraftingInterpreters.Lox
{
    public class Parser
    {
        public class ParseError : Exception
        {
            public ParseError(string message) : base(message)
            {
            }
        }
        private readonly List<Token> tokens;
        private int current = 0;

        public Parser(List<Token> tokens)
        {
            //Console.WriteLine("Parsing 1");

            this.tokens = tokens;
        }

        // public Expr Parse()
        // {
            public List<Stmt> Parse()
            {
                //Console.WriteLine("Parsing 2");

                List<Stmt> statements = new List<Stmt>();
                while (!IsAtEnd())
                {
                    statements.Add(Declaration());
                }

                return statements;
            }

        // }

        private Expr Expression() {
            //Console.WriteLine("Expression!!!!!");

            return Assignment();
        }

        private Stmt Declaration()
        {
            try
            {
                if (Match(TokenType.FUN))
                {
                    return Function("function");
                }

                if (Match(TokenType.VAR)){
                    // Console.WriteLine("Declaration");
                    return VarDeclaration();
                } 
            
                return Statement();
            }
            catch (ParseError error)
            {
                Synchronize();
                return null;
            }
        }

        private Stmt Statement()
        {
            if (Match(TokenType.FOR)) return ForStatement();
            if (Match(TokenType.IF)) return IfStatement();
            if (Match(TokenType.PRINT)) return PrintStatement();
            if (Match(TokenType.RETURN)) return ReturnStatement();
            if (Match(TokenType.WHILE)) return WhileStatement();
            if (Match(TokenType.LEFT_BRACE)) return new Stmt.Block(Block());


            return ExpressionStatement();
        }

        private Stmt IfStatement()
        {
            Consume(TokenType.LEFT_PAREN, "Expect '(' after 'if'.");
            Expr condition = Expression();
            Consume(TokenType.RIGHT_PAREN, "Expect ')' after if condition.");

            Stmt thenBranch = Statement();
            Stmt elseBranch = null;
            if (Match(TokenType.ELSE))
            {
                elseBranch = Statement();
            }

            return new Stmt.If(condition, thenBranch, elseBranch);
        }

        private Stmt ForStatement()
        {
            Consume(TokenType.LEFT_PAREN, "Expect '(' after 'for'.");

            // Parse initializer
            Stmt initializer;
            if (Match(TokenType.SEMICOLON))
            {
                initializer = null;
            }
            else if (Match(TokenType.VAR))
            {
                // Console.WriteLine("Match VAR");
                initializer = VarDeclaration();
            }
            else
            {
                initializer = ExpressionStatement();
            }

            // Parse condition
            Expr condition = null;
            if (!Check(TokenType.SEMICOLON))
            {
                condition = Expression();
            }
            Consume(TokenType.SEMICOLON, "Expect ';' after loop condition.");

            // Parse increment
            Expr increment = null;
            if (!Check(TokenType.RIGHT_PAREN))
            {
                increment = Expression();
            }
            Consume(TokenType.RIGHT_PAREN, "Expect ')' after for clauses.");

            // Parse body
            Stmt body = Statement();

            if (increment != null)
            {
                body = new Stmt.Block(new List<Stmt>{body, new Stmt.Expression(increment)});
            }

            if (condition == null)
            {
                condition = new Expr.Literal(true);
            }

            body = new Stmt.While(condition, body);

            if (initializer != null)
            {
                body = new Stmt.Block(new List<Stmt> { initializer, body });
            }
            // Create and return the For statement
            return body;
        }


        private Stmt PrintStatement()
        {
            //Console.WriteLine("Visiting PrintStmt");


            Expr value = Expression();
            Consume(TokenType.SEMICOLON, "Expect ';' after value.");


            return new Stmt.Print(value);
        }

        private Stmt ReturnStatement()
        {
            //Console.WriteLine("Parsing return statement");

            Token keyword = Previous();
            Expr value = null;
            if (!Check(TokenType.SEMICOLON))
            {
                value = Expression();
            }

            Consume(TokenType.SEMICOLON, "Expect ';' after return value.");
            return new Stmt.Return(keyword, value);
        }

        private Stmt ExpressionStatement()
        {
            //Console.WriteLine("Parsing expression statement");

            Expr expr = Expression();
            Consume(TokenType.SEMICOLON, "Expect ';' after expression.");
            return new Stmt.Expression(expr);
        }

        private Stmt.Function Function(string kind)
        {
            //Console.WriteLine($"Parsing {kind} function");

            Token name = Consume(TokenType.IDENTIFIER, $"Expect {kind} name.");
            Consume(TokenType.LEFT_PAREN, $"Expect '(' after {kind} name.");
            List<Token> parameters = new List<Token>();
            if (!Check(TokenType.RIGHT_PAREN))
            {
                do
                {
                    if (parameters.Count >= 255)
                    {
                        Error(Peek(), "Can't have more than 255 parameters.");
                    }

                    parameters.Add(
                        Consume(TokenType.IDENTIFIER, "Expect parameter name."));
                } while (Match(TokenType.COMMA));
            }
            Consume(TokenType.RIGHT_PAREN, "Expect ')' after parameters.");
            Consume(TokenType.LEFT_BRACE, $"Expect '{{' before {kind} body.");
            List<Stmt> body = Block();
            return new Stmt.Function(name, parameters, body);

        }


        private List<Stmt> Block()
        {
            //Console.WriteLine("Visiting Block");

            List<Stmt> statements = new List<Stmt>();

            while (!Check(TokenType.RIGHT_BRACE) && !IsAtEnd())
            {
                //Console.WriteLine("Parsing block statement");

                statements.Add(Declaration());
            }

            Consume(TokenType.RIGHT_BRACE, "Expect '}' after block.");
            return statements;
        }

        private Expr Assignment()
        {
            //Console.WriteLine("Parsing assignment");

            Expr expr = Or();

            if (Match(TokenType.EQUAL))
            {
                // Console.WriteLine("HERE?");

                Token equals = Previous();
                Expr value = Assignment();

                if (expr is Expr.Variable variableExpr)
                {
                    // Console.WriteLine("HERHELLOOOOE?");
                    var name = variableExpr.Name;
                    // Console.WriteLine($"Assigning value to variable '{name.Lexeme}'");
                    return new Expr.Assign(name, value);
                }

                Error(equals, "Invalid assignment target.");
            }

            return expr;
        }

        private Expr Or()
        {
            //Console.WriteLine("Got to OR");

            Expr expr = And();

            while (Match(TokenType.OR))
            {
                //Console.WriteLine("Got inside OR");

                Token @operator = Previous();
                Expr right = And();
                expr = new Expr.Logical(expr, @operator, right);
            }

            return expr;
        }

        private Expr And()
        {
            //Console.WriteLine("Got to AND");

            Expr expr = Equality();

            while (Match(TokenType.AND))
            {
                Token @operator = Previous();
                Expr right = Equality();
                expr = new Expr.Logical(expr, @operator, right);
            }

            return expr;
        }

        private Stmt VarDeclaration()
        {
            // Console.WriteLine("Parsing var declaration");
            Token name = Consume(TokenType.IDENTIFIER, "Expect variable name.");

            Expr initializer = null;
            if (Match(TokenType.EQUAL))
            {
                // Console.WriteLine("Match EQUAL");
                initializer = Expression();
            }

            Consume(TokenType.SEMICOLON, "Expect ';' after variable declaration.");
            return new Stmt.Var(name, initializer);
        }

        private Stmt WhileStatement()
        {
            Consume(TokenType.LEFT_PAREN, "Expect '(' after 'while'.");
            Expr condition = Expression();
            Consume(TokenType.RIGHT_PAREN, "Expect ')' after condition.");
            Stmt body = Statement();

            return new Stmt.While(condition, body);
        }

        private Expr Comparison()
        {
            //Console.WriteLine("Got to COMPARISON");

            var expr = Term();

            while (Match(TokenType.GREATER, TokenType.GREATER_EQUAL, TokenType.LESS, TokenType.LESS_EQUAL))
            {
                var op = Previous();
                var right = Term();
                expr = new Expr.Binary(expr, op, right);
            }

            return expr;
        }

        private Expr Term()
        {
            //Console.WriteLine("Got to TERM");

            var expr = Factor();

            while (Match(TokenType.MINUS, TokenType.PLUS))
            {
                var op = Previous();
                var right = Factor();
                expr = new Expr.Binary(expr, op, right);
            }

            return expr;
        }

        private Expr Factor()
        {
            //Console.WriteLine("Got to FACTOR");

            var expr = Unary();

            while (Match(TokenType.SLASH, TokenType.STAR))
            {
                var op = Previous();
                var right = Unary();
                expr = new Expr.Binary(expr, op, right);
            }

            return expr;
        }

        private Expr Unary()
        {
            //Console.WriteLine("Got to UNARY");

            if (Match(TokenType.BANG, TokenType.MINUS))
            {
                var op = Previous();
                var right = Unary();
                return new Expr.Unary(op, right);
            }

            return Call();
        }

        private Expr Call()
        {
            //Console.WriteLine("Got to CALL");

            Expr expr = Primary();

            while (true)
            {
                if (Match(TokenType.LEFT_PAREN))
                {
                    expr = FinishCall(expr);
                }
                else
                {
                    break;
                }
            }

            return expr;
        }

        private Expr FinishCall(Expr callee)
        {
            List<Expr> arguments = new List<Expr>();
            if (!Check(TokenType.RIGHT_PAREN))
            {
                do
                {
                    if (arguments.Count >= 255) {
                        Error(Peek(), "Can't have more than 255 arguments.");
                    }
                    arguments.Add(Expression());
                } while (Match(TokenType.COMMA));
            }

            Token paren = Consume(TokenType.RIGHT_PAREN, "Expect ')' after arguments.");

            return new Expr.Call(callee, paren, arguments);
        }


        private Expr Primary()
        {
            //Console.WriteLine("Got to PRIMARY");

            if (Match(TokenType.FALSE))
            {
                //Console.WriteLine("Parsing FALSE");
                return new Expr.Literal(false);
            }

            if (Match(TokenType.TRUE))
            {
                //Console.WriteLine("Parsing TRUE");
                return new Expr.Literal(true);
            }

            if (Match(TokenType.NIL))
            {
                //Console.WriteLine("Parsing NIL");
                return new Expr.Literal(null);
            }

            if (Match(TokenType.NUMBER, TokenType.STRING))
            {
                //Console.WriteLine("Parsing NUMBER or STRING");
                return new Expr.Literal(Previous().Literal);
            }

            if (Match(TokenType.IDENTIFIER))
            {
                //Console.WriteLine("Parsing IDENTIFIER");
                return new Expr.Variable(Previous());
            }

            if (Match(TokenType.LEFT_PAREN))
            {
                //Console.WriteLine("Parsing LEFT_PAREN");
                Expr expr = Expression();
                Consume(TokenType.RIGHT_PAREN, "Expect ')' after expression.");
                //Console.WriteLine("Parsing RIGHT_PAREN");
                return new Expr.Grouping(expr);
            }

            throw Error(Peek(), "Expect expression.");
        }


        private Expr Equality()
        {
            //Console.WriteLine("Got to EQUALITY");

            var expr = Comparison();

            while (Match(TokenType.BANG_EQUAL, TokenType.EQUAL_EQUAL))
            {
                var _operator = Previous();
                var right = Comparison();
                expr = new Expr.Binary(expr, _operator, right);
            }

            return expr;
        }

        private bool Match(params TokenType[] types)
        {
            foreach (var type in types)
            {
                if (Check(type))
                {
                    Advance();
                    return true;
                }
            }

            return false;
        }

        private Token Consume(TokenType type, string message)
        {
            // Console.WriteLine("Consuming");

            if (Check(type)) return Advance();
            return null;
//REMOVING THIS LINE SO I DONT ERROR OUT EVERYTIME I DONT PUT A SEMICOLON >:(
            // throw Error(Peek(), message);
        }

        private bool Check(TokenType type)
        {
            // Console.WriteLine("Is at end check?");

            if (IsAtEnd()) return false;
            return Peek().Type == type;
        }

        private Token Advance()
        {
            // Console.WriteLine("Advancing");

            if (!IsAtEnd()) current++;
            return Previous();
        }

        private bool IsAtEnd()
        {
            // Console.WriteLine("Is at end?");

            return Peek().Type == TokenType.EOF;
        }

        private Token Peek()
        {
            return tokens[current];
        }

        private Token Previous()
        {
            return tokens[current - 1];
        }

        private ParseError Error(Token token, string message)
        {
            Console.WriteLine("Erroring");
            Lox.Error(token, message);
            return new ParseError(message);
        }
        private void Synchronize()
        {
            Advance();

            while (!IsAtEnd())
            {
                if (Previous().Type == TokenType.SEMICOLON)
                    return;

                switch (Peek().Type)
                {
                    case TokenType.CLASS:
                    case TokenType.FUN:
                    case TokenType.VAR:
                    case TokenType.FOR:
                    case TokenType.IF:
                    case TokenType.WHILE:
                    case TokenType.PRINT:
                    case TokenType.RETURN:
                        return;
                }

                Advance();
            }
        }


    }

    
}