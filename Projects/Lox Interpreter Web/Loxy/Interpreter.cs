using System;
using System.Collections.Generic;


namespace CraftingInterpreters.Lox
{
public class Interpreter : Expr.IVisitor<object>, Stmt.IVisitor<object>    {
    private Environment Globals;
        private Environment environment;

        public Interpreter()
        {
            //Console.WriteLine("Entering Interpreter Global");

            Globals = new Environment();
            environment = Globals;

            Globals.Define("clock", new Clock());
            //Console.WriteLine("Exiting Interpreter Global");
        }

        public void Interpret(List<Stmt> statements)
        {
            //Console.WriteLine("Entering Interpret");

            try
            {
                //Console.WriteLine("Visiting Try statement");

                foreach (Stmt statement in statements)
                {
                    //Console.WriteLine("Visiting Foreach statement");

                    Execute(statement);
                }
            }
            catch (RuntimeError error)
            {
                Lox.RuntimeError(error);
            }
        }
        public object VisitLiteralExpr(Expr.Literal expr)
        {
            //Console.WriteLine("Visiting Visit Literal expression");
            return expr.Value;
        }

        public object VisitUnaryExpr(Expr.Unary expr)
        {
            object right = Evaluate(expr.Right);

            switch (expr.Operator.Type)
            {
                case TokenType.MINUS:
                    CheckNumberOperand(expr.Operator, right);
                    return -(double)right;
                case TokenType.BANG:
                    return !isTruthy(right);
            }

            // Unreachable.
            return null;
        }

        public object VisitVariableExpr(Expr.Variable expr)
        {
            return environment.Get(expr.Name);
        }

        private void CheckNumberOperand(Token _operator, object operand)
        {
            if (operand is double)
                return;

            throw new RuntimeError(_operator, "Operand must be a number.");
        }

        private bool isTruthy(object obj)
        {
            if (obj == null) return false;
            if (obj is bool booleanValue) return booleanValue;
            return true;
        }

        private bool IsEqual(object left, object right)
        {
            if (left == null && right == null)
                return true;

            if (left == null || right == null)
                return false;

            return left.Equals(right);
        }

        private string Stringify(object obj)
        {
            if (obj == null) return "nil";

            if (obj is double)
            {
                string text = obj.ToString();
                if (text.EndsWith(".0"))
                {
                    text = text.Substring(0, text.Length - 2);
                }
                return text;
            }

            return obj.ToString();
        }
       
        public object VisitGroupingExpr(Expr.Grouping expr)
        {
            return Evaluate(expr.Expression);
        }

         private object Evaluate(Expr expr)
        {
            //Console.WriteLine($"Evaluating expression: {expr}");
            return expr.Accept(this);
        }

        private void Execute(Stmt stmt)
        {
            //Console.WriteLine($"Executing statement: {stmt}");
            stmt.Accept(this);
        }

        public void ExecuteBlock(List<Stmt> statements, Environment environment)
        {
            //Console.WriteLine("Visiting Execute Block statement");

            Environment previous = this.environment;
            try
            {
                this.environment = environment;

                foreach (Stmt statement in statements)
                {
                    Execute(statement);
                }
            }
            finally
            {
                this.environment = previous;
            }
        }

        public object VisitBlockStmt(Stmt.Block stmt)
        {
            //Console.WriteLine("Visiting Block statement");

            ExecuteBlock(stmt.statements, new Environment(environment));
            return null;
        }

         public object VisitExpressionStmt(Stmt.Expression stmt)
        {
            Evaluate(stmt.expression);
            return null;
        }

          public object VisitPrintStmt(Stmt.Print stmt)
        {
            //Console.WriteLine("Visiting Print statement");
            object value = Evaluate(stmt.Expression);
            Console.WriteLine(Stringify(value));
            return null;
        }

        public object VisitVarStmt(Stmt.Var stmt)
        {
            //Console.WriteLine("Visiting Var statement");

            // Check if there is an initializer
            if (stmt.initializer != null)
            {
                // Evaluate the initializer expression
                object value = Evaluate(stmt.initializer);

                // Define the variable with the evaluated value
                environment.Define(stmt.Name.Lexeme, value);
                //Console.WriteLine($"Variable {stmt.Name.Lexeme} initialized with value: {Stringify(value)}");
            }
            else
            {
                // If no initializer, define the variable without a value
                environment.Define(stmt.Name.Lexeme, null);
                //Console.WriteLine($"Variable {stmt.Name.Lexeme} defined without an initializer");
            }

            return null;
        }
    

        public object VisitAssignExpr(Expr.Assign expr)
        {
            object value = Evaluate(expr.Value);
            environment.Assign(expr.Name, value);
            return value;
        }

        public object VisitBinaryExpr(Expr.Binary expr)
        {
            object left = Evaluate(expr.Left);
            object right = Evaluate(expr.Right);

            switch (expr.Operator.Type)
            {
                case TokenType.GREATER:
                    CheckNumberOperands(expr.Operator, left, right);
                    return (double)left > (double)right;
                case TokenType.GREATER_EQUAL:
                    CheckNumberOperands(expr.Operator, left, right);
                    return (double)left >= (double)right;
                case TokenType.LESS:
                    CheckNumberOperands(expr.Operator, left, right);
                    return (double)left < (double)right;
                case TokenType.LESS_EQUAL:
                    CheckNumberOperands(expr.Operator, left, right);
                    return (double)left <= (double)right;
                case TokenType.MINUS:
                    CheckNumberOperands(expr.Operator, left, right);
                    return (double)left - (double)right;
                case TokenType.PLUS:
                    CheckNumberOperands(expr.Operator, left, right);
                    if (left is double && right is double)
                    {
                        return (double)left + (double)right;
                    }

                    if (left is string && right is string)
                    {
                        return (string)left + (string)right;
                    }
                    throw new RuntimeError(expr.Operator, "Operands must be two numbers or two strings.");
                case TokenType.SLASH:
                    CheckNumberOperands(expr.Operator, left, right);
                    return (double)left / (double)right;
                case TokenType.STAR:
                    CheckNumberOperands(expr.Operator, left, right);
                    return (double)left * (double)right;
                case TokenType.BANG_EQUAL:
                    return !IsEqual(left, right);
                case TokenType.EQUAL_EQUAL:
                    return IsEqual(left, right);
            }

            // Unreachable.
            return null;
        }

        public object VisitCallExpr(Expr.Call expr)
        {
            object callee = Evaluate(expr.Callee);

            List<object>  arguments = new List<object>();
            foreach (Expr argument in expr.Arguments)
            {
                arguments.Add(Evaluate(argument));
            }

            if (!(callee is LoxCallable))
            {
                throw new RuntimeError(expr.Paren, "Can only call functions and classes.");
            }

            LoxCallable function = (LoxCallable)callee;

            if (arguments.Count != function.Arity())
            {
                throw new RuntimeError(expr.Paren, $"Expected {function.Arity()} arguments but got {arguments.Count}.");
            }

            return function.Call(this, arguments);
        }

        // evaluates get expressions (getters)
        public object VisitGetExpr(Expr.Get expr)
        {
            return null;
        }

        public object VisitLogicalExpr(Expr.Logical expr)
        {
            object left = Evaluate(expr.Left);

            if (expr.Operator.Type == TokenType.OR)
            {
                if (isTruthy(left)) return left;
            }
            else
            {
                if (!isTruthy(left)) return left;
            }

            return Evaluate(expr.Right);
        }

        // evaluates set expressions (setters)
        public object VisitSetExpr(Expr.Set expr)
        {
            return null;
        }
        public object VisitFunctionStmt(Stmt.Function stmt)
        {
            LoxFunction function = new LoxFunction(stmt, environment);
            environment.Define(stmt.Name.Lexeme, function);
            return null;
        }

        public object VisitIfStmt(Stmt.If stmt)
        {
            if (isTruthy(Evaluate(stmt.condition)))
                Execute(stmt.thenBranch);
            else if (stmt.elseBranch != null) Execute(stmt.elseBranch);

            return null;
        }

        public object VisitReturnStmt(Stmt.Return stmt)
        {
            object value = null;
            if (stmt.Value != null)
                value = Evaluate(stmt.Value);

            throw new Return(value);
        }

        public object VisitWhileStmt(Stmt.While stmt)
        {
            while (isTruthy(Evaluate(stmt.condition)))
            {
                Execute(stmt.Body);
            }

            return null;
        }
        private void CheckNumberOperands(Token @operator, object left, object right)
        {
            if (left is double && right is double) return;

            throw new RuntimeError(@operator, "Operands must be numbers.");
        }


    }


}
