using System;
using CraftingInterpreters.Lox;


namespace CraftingInterpreters.Lox
{
    public class RuntimeError : Exception
    {
        public Token Token { get; }

        public RuntimeError(Token token, string message) : base(message)
        {
            Token = token;
        }
    }
}
