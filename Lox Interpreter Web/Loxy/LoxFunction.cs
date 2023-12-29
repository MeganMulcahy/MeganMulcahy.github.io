using System;
using System.Collections.Generic;

namespace CraftingInterpreters.Lox
{
    class LoxFunction : LoxCallable
    {
        private readonly Stmt.Function declaration;
        private readonly Environment closure;

        public LoxFunction(Stmt.Function declaration, Environment closure)
        {
            this.closure = closure;
            this.declaration = declaration;
        }

        public int Arity()
        {
            return declaration.parms.Count;
        }

        public void SetArguments(Environment environment, List<object> arguments)
        {
            for (int i = 0; i < declaration.parms.Count; i++)
            {
                environment.Define(declaration.parms[i].Lexeme, arguments[i]);
            }
        }

        public object Call(Interpreter interpreter, List<object> arguments)
        {
            Environment environment = new Environment(closure);

            SetArguments(environment, arguments);

            try
            {
                interpreter.ExecuteBlock(declaration.Body, environment);
            }
            catch (Return returnValue)
            {
                return returnValue.Value;
            }

            return null;
        }

        public override string ToString()
        {
            return "<fn " + declaration.Name.Lexeme + ">";
        }
    }
}