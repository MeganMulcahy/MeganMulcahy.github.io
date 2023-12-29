using System;
using System.Collections.Generic;

namespace CraftingInterpreters.Lox
{
    public abstract class Stmt
    {
        public abstract T Accept<T>(IVisitor<T> visitor);

        // IVisitor interface with all relevant statement types
        public interface IVisitor<T>
        {
            T VisitBlockStmt(Block stmt);
            T VisitExpressionStmt(Expression stmt);
            T VisitFunctionStmt(Function stmt);
            T VisitIfStmt(If stmt);
            T VisitPrintStmt(Print stmt);
            T VisitReturnStmt(Return stmt);
            T VisitVarStmt(Var stmt);
            T VisitWhileStmt(While stmt);
        }

        // defines block statements
        public class Block : Stmt
        {
            public List<Stmt> statements;

            public Block(List<Stmt> statements)
            {
                this.statements = statements;
            }

            public override T Accept<T>(IVisitor<T> visitor)
            {
                return visitor.VisitBlockStmt(this);
            }
        }

        // defines expression statements
        public class Expression : Stmt
        {
            public Expr expression;

            public Expression(Expr expression)
            {
                //Console.WriteLine("Reached the Expression constructor"); // Add this line

                this.expression = expression;
            }

            public override T Accept<T>(IVisitor<T> visitor)
            {
                //Console.WriteLine("Reached the Expression Stmt"); // Add this line

                return visitor.VisitExpressionStmt(this);
            }
        }

        // defines function statements
        public class Function : Stmt
        {
            public List<Stmt> Body;

            public Token Name;
            public List<Token> parms;

            public Function(Token name, List<Token> parms, List<Stmt> body)
            {
                this.Name = name;
                this.parms = parms;
                this.Body = body;
            }

            public override T Accept<T>(IVisitor<T> visitor)
            {
                return visitor.VisitFunctionStmt(this);
            }
        }

        // defines if statements
        public class If : Stmt
        {
            public Expr condition;
            public Stmt elseBranch;
            public Stmt thenBranch;

            public If(Expr condition, Stmt thenBranch, Stmt elseBranch)
            {
                this.condition = condition;
                this.thenBranch = thenBranch;
                this.elseBranch = elseBranch;
            }

            public override T Accept<T>(IVisitor<T> visitor)
            {
                return visitor.VisitIfStmt(this);
            }
        }

        // defines print statements

        public class Print : Stmt
        {
            public Expr Expression;

            public Print(Expr expression)
            {
                //Console.WriteLine("Reached the Print constructor"); // Add this line

                this.Expression = expression;
            }

            public override T Accept<T>(IVisitor<T> visitor)
            {
                //Console.WriteLine("Visiting PrintStmt"); // Add this line

                return visitor.VisitPrintStmt(this);
            }
        }

        // defines return statements
        public class Return : Stmt
        {
            public readonly Token Keyword;
            public readonly Expr Value;

            public Return(Token keyword, Expr value)
            {
                this.Keyword = keyword;
                this.Value = value;
            }

            public override T Accept<T>(IVisitor<T> visitor)
            {
                return visitor.VisitReturnStmt(this);
            }
        }

        // defines variable statements
        public class Var : Stmt
        {
            public Expr initializer;

            public Token Name;

            public Var(Token name, Expr initializer)
            {
                this.Name = name;
                this.initializer = initializer;
            }

            public override T Accept<T>(IVisitor<T> visitor)
            {
                return visitor.VisitVarStmt(this);
            }
        }

        // defines while statements
        public class While : Stmt
        {
            public Stmt Body;

            public Expr condition;

            public While(Expr condition, Stmt body)
            {
                this.condition = condition;
                this.Body = body;
            }

            public override T Accept<T>(IVisitor<T> visitor)
            {
                return visitor.VisitWhileStmt(this);
            }
        }
    }
}