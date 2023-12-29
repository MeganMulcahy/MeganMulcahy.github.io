namespace CraftingInterpreters.Lisp
{
public class Printer
{
    private SExpr SExpr;

    public Printer(SExpr sexpr)
    {
        SExpr = sexpr;
    }

    public string Print(SExpr sexpr)
{
    if (sexpr is SExpr.Atom)
    {
        SExpr.Atom atom = (SExpr.Atom)sexpr;
        Token t = (Token)atom.Value;
        return $"{t.Lexeme} ";
    }
    else if (sexpr is SExpr.Null)
    {
        return "()";
    }
    else
    {
        // sexpr is list
        SExpr.ListExpr listexpr = (SExpr.ListExpr)sexpr;

        string output = "(";

        // Include the result of Print(listexpr.Left) in the output
        output += Print(listexpr.Left);

        foreach (SExpr s in listexpr.sexprs)
        {
            output += " ";
            output += Print(s);
        }

        return output + ")";
    }
}

// public override string ToString()
// {
//     // Call Print method with the current instance of SExpr
//     return Print(this);
// }

}
}
