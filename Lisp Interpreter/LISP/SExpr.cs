using System;
using System.Text;
using System.Collections.Generic;

namespace CraftingInterpreters.Lisp
{
    
    public abstract class SExpr
    {
        public Token Value;

        public class ListExpr : SExpr {
            public SExpr Left; // no nils
            public List<SExpr> sexprs;
            public ListExpr(SExpr left, List<SExpr> sexprs) {
                Left = left;
                this.sexprs = sexprs;
            }
        }

        public class Atom : SExpr {
            public Atom(Token value) {
                Value = value;
            }
        }

        public class Null : SExpr {
            public Null() { }

            public Object value = null;
        }
        public override string ToString()
{
    if (this is Atom atom)
    {
        Token t = atom.Value;
        return $"{t.Lexeme}";
    }
    else if (this is Null)
    {
        return "nil";
    }
    else if (this is ListExpr listexpr)
    {
        StringBuilder output = new StringBuilder("(");

        output.Append(listexpr.Left.ToString());

        foreach (SExpr s in listexpr.sexprs)
        {
            output.Append(" ");
            output.Append(s.ToString());
        }

        output.Append(")");

        return output.ToString();
    }

    // Handle other cases if needed
    return base.ToString();
}

    }
}
