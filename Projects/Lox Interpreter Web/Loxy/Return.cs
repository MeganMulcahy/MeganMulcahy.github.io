using System;
using CraftingInterpreters.Lox;

namespace CraftingInterpreters.Lox
{

    public class Return : Exception
    {
        public readonly object Value;

        public Return(object val)
        {
            this.Value = val;
        }
    }
}