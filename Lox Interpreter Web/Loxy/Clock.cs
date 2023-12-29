using System;
using System.Collections.Generic;

namespace CraftingInterpreters.Lox
{

    public class Clock : LoxCallable
    {
        public int Arity()
        {
            //Console.WriteLine("Setting Arity");

            return 0;
        }

        public object Call(Interpreter interpreter, List<object> arguments)
        {
            //Console.WriteLine("Setting Call");

            var millisec = DateTime.Now.Ticks / TimeSpan.TicksPerMillisecond;
            return millisec / 1000.0;
        }

        public override string ToString()
        {
            //Console.WriteLine("Setting ToString");

            return "<native fn>";
        }
    }
}