using System;

namespace CraftingInterpreters.Lisp
{
    public class CADRCell
    {
        public object Car { get; set; }
        public object Cdr { get; set; }

        public CADRCell(object car, object cdr)
        {
            Car = car;
            Cdr = cdr;
        }

        public CADRCell(Object car) 
        {
            Car = car;
            Cdr = new SExpr.Null();
        }

        public void Print()
        {
            Console.Write("(");

            if (Car != null)
            {
                PrintRecursive(Car);
                PrintRecursive(Cdr);
            }

            Console.Write(")");
        }

        public void PrintRecursive(object obj)
        {
            if (obj is CADRCell cell)
            {
                Console.Write(" ");
                PrintRecursive(cell.Car);
                PrintRecursive(cell.Cdr);
                return;
            }

            if (obj is SExpr.ListExpr list)
            {
                Console.Write(" ");
                if (list.sexprs.Count > 0)
                {
                    // Print only the head of the list
                    PrintRecursive(list.sexprs[0]);
                }
                return;
            }

            if (obj is SExpr.Null)
            {
                // If it's null, don't print anything
                return;
            }

            Console.Write(obj);
        }

        public override string ToString()
        {
            return $"CADRCell({Car}, {Cdr})";
        }

        public void PrintList(SExpr.ListExpr list)
        {
            foreach (var sexpr in list.sexprs)
            {
                Console.Write(" ");
                PrintRecursive(sexpr);
            }
        }
    }
}
