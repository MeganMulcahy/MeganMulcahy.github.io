using System.Collections.Generic;

namespace CraftingInterpreters.Lox
{
    public interface LoxCallable
    {
        int Arity();

        object Call(Interpreter interpreter, List<object> arguments);
    }
}
