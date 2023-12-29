using System;
using System.Collections.Generic;


namespace CraftingInterpreters.Lox
{
    public abstract class Expr
    {
        public interface IVisitor<T>
        {
            T VisitAssignExpr(Assign expr);
            T VisitBinaryExpr(Binary expr);
            T VisitGroupingExpr(Grouping expr);
            T VisitLiteralExpr(Literal expr);
            T VisitUnaryExpr(Unary expr);
            T VisitVariableExpr(Variable expr);
            T VisitCallExpr(Call expr);
            T VisitGetExpr(Get expr);
            T VisitLogicalExpr(Logical expr);
            T VisitSetExpr(Set expr);
        }

        public abstract T Accept<T>(IVisitor<T> visitor);

        public class Assign : Expr
        {
            public Token Name;
            public Expr Value;

            public Assign(Token name, Expr value)
            {
                this.Name = name;
                this.Value = value;
            }

            public override T Accept<T>(IVisitor<T> visitor)
            {
                return visitor.VisitAssignExpr(this);
            }
        }
        // Define the missing types here
        public class Literal : Expr
        {

            public object Value { get; }

            public Literal(object value)
            {
                //Console.WriteLine($"Entered Literal Expression with value: {value}");

                Value = value;
            }

            public override T Accept<T>(IVisitor<T> visitor)
            {
                //Console.WriteLine("Entered Literal Accept");

                return visitor.VisitLiteralExpr(this);
            }
        }

        public class Unary : Expr
        {
            public Token Operator { get; }
            public Expr Right { get; }

            public Unary(Token @operator, Expr right)
            {
                Operator = @operator;
                Right = right;
            }

            public override T Accept<T>(IVisitor<T> visitor)
            {
                return visitor.VisitUnaryExpr(this);
            }
        }

        public class Grouping : Expr
        {
            public Expr Expression { get; }

            public Grouping(Expr expression)
            {
                Expression = expression;
            }

            public override T Accept<T>(IVisitor<T> visitor)
            {
                return visitor.VisitGroupingExpr(this);
            }
        }
        public class Binary : Expr
        {
            public Expr Left { get; }
            public Token Operator { get; }
            public Expr Right { get; }

            public Binary(Expr left, Token @operator, Expr right)
            {
                Left = left;
                Operator = @operator;
                Right = right;
            }

            public override T Accept<T>(IVisitor<T> visitor)
            {
                return visitor.VisitBinaryExpr(this);
            }
        }
        public class Variable : Expr
        {
            public Token Name;

            public Variable(Token name)
            {
                this.Name = name;
            }

            public override T Accept<T>(IVisitor<T> visitor)
            {
                return visitor.VisitVariableExpr(this);
            }
        }
   
        public class Call : Expr
        {
            public List<Expr> Arguments;

            public Expr Callee;
            public Token Paren;

            public Call(Expr callee, Token paren, List<Expr> arguments)
            {
                this.Callee = callee;
                this.Paren = paren;
                this.Arguments = arguments;
            }

            public override T Accept<T>(IVisitor<T> visitor)
            {
                return visitor.VisitCallExpr(this);
            }
        }

        // get expression (getter) definition
        public class Get : Expr
        {
            public Token Name;

            public Expr Object;

            public Get(Expr obj, Token name)
            {
                this.Object = obj;
                this.Name = name;
            }

            public override T Accept<T>(IVisitor<T> visitor)
            {
                return visitor.VisitGetExpr(this);
            }
        }
        public class Logical : Expr
        {
            public Expr Left;
            public Token Operator;
            public Expr Right;

            public Logical(Expr left, Token op, Expr right)
            {
                this.Left = left;
                this.Operator = op;
                this.Right = right;
            }

            public override T Accept<T>(IVisitor<T> visitor)
            {
                return visitor.VisitLogicalExpr(this);
            }
        }

        // set expression (setter) definition
        public class Set : Expr
        {
            public Token Name;

            public Expr Object;
            public Expr Value;

            public Set(Expr obj, Token name, Expr value)
            {
                //Console.WriteLine("Visiting Set Expression");

                this.Object = obj;
                this.Name = name;
                this.Value = value;
            }

            public override T Accept<T>(IVisitor<T> visitor)
            {
                //Console.WriteLine("Visiting Assign Set Expression");

                return visitor.VisitSetExpr(this);
            }
        }
    }
}
