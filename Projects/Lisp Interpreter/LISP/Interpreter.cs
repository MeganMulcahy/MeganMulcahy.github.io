using System;
using System.Text;
using System.Linq;
using System.Collections.Generic;

namespace CraftingInterpreters.Lisp
{
public class Interpreter
{
    List<SExpr> Sexprs;
    Dictionary<string, Object> vars = new Dictionary<string, Object>();

    public Interpreter(List<SExpr> sexprs)
    {
        Sexprs = sexprs;
    }

    public void interpret()
{
    int index = 0;
    while (index < Sexprs.Count)
    {
        Object result = evaluate(Sexprs[index]);

        switch (result)
        {
            case CADRCell cell:
                cell.Print();
                Console.WriteLine();
                break;

            case SExpr.Null:
                Console.WriteLine("()");
                break;

            default:
                Console.WriteLine(result);
                break;
        }

        index++;  // Move this line inside the loop
    }
}

    // private Object evaluate(SExpr sexpr) {
    //     if (sexpr is SExpr.ListExpr) { return evaluate((SExpr.ListExpr)sexpr); }
    //     else if (sexpr is SExpr.Atom) { return evaluate((SExpr.Atom)sexpr); }
    //     else { return evaluate((SExpr.Null)sexpr); }
    // }

    private Object evaluate(SExpr sexpr)
    {
        if (sexpr is SExpr.ListExpr)
        {
            return evaluate((SExpr.ListExpr)sexpr);
        }
        else if (sexpr is SExpr.Atom)
        {
            return evaluate((SExpr.Atom)sexpr);
        }
        else
        {
            return evaluate((SExpr.Null)sexpr);
        }
    }
    private Object evaluate(SExpr.ListExpr list) {
        Object left = list.Left; // atom or listexpr

        if (left is SExpr.Atom)
        {
            // only one atom in the list
            left = evaluate((SExpr.Atom)left);
        }
        else
        { 
            // left is a list
            left = evaluate((SExpr.ListExpr)left);
        }

        // list
        if (left is string)
        {
            switch ((string)left)
            {
                //operators
                case "+":
                    return (int)evaluate(list.sexprs[0]) + (int)evaluate(list.sexprs[1]);
                case "-":
                    return (int)evaluate(list.sexprs[0]) - (int)evaluate(list.sexprs[1]);
                case "*":
                    return (int)evaluate(list.sexprs[0]) * (int)evaluate(list.sexprs[1]);
                case "/":
                    return (int)evaluate(list.sexprs[0]) / (int)evaluate(list.sexprs[1]);
                case "=":
                case "eq?":
                    return eq(list.sexprs);
                case ">":
                    return (int)evaluate(list.sexprs[0]) > (int)evaluate(list.sexprs[1]);
                case "<":
                    return (int)evaluate(list.sexprs[0]) < (int)evaluate(list.sexprs[1]);

                //definitions/semantics
                case "define":
                    return define(list.sexprs);
                case "set":
                    return set(list.sexprs);
                // case "cadr":
                //     return cadr(list.sexprs);
                case "number?":
                    return evaluate(list.sexprs[0]) is int;
                case "symbol?":
                    return symbol(list.sexprs[0]);
                case "cons":
                    return cons(list.sexprs);
                case "if":
                    return ify(list.sexprs);
                case "car":
                    return car(list.sexprs);
                case "cdr":
                    return cdr(list.sexprs);
                case "and":
                    return truth(list.sexprs[0]) && truth(list.sexprs[1]);
                case "or":
                    return truth(list.sexprs[0]) || truth(list.sexprs[1]);
                case "not":
                    return !truth(list.sexprs[0]);
                case "list?":
                    return this.list(list.sexprs[0]);
                case "list":
                    return printingList(list.sexprs);
                case "nil?":
                    return !truth(list.sexprs[0]);
                
                default:
                    List<SExpr> info = (List<SExpr>)vars[(string)left];
                    SExpr.ListExpr parameters = (SExpr.ListExpr)info[0];   // list of symbols
                    SExpr body = info[1];  // sexpr to evaluate

                    vars[((SExpr.Atom)parameters.Left).Value.Lexeme] = evaluate(list.sexprs[0]); // directly cast listexpr to Atom, since we know the parameter list is a list of symbols
                    for (int i = 0; i < parameters.sexprs.Count; i++) {
                        vars[((SExpr.Atom)parameters.sexprs[i]).Value.Lexeme] = evaluate(list.sexprs[i + 1]);
                    }

                    Object ret = evaluate(body);

                    vars.Remove(((SExpr.Atom)parameters.Left).Value.Lexeme);
                    foreach (SExpr sexpr in parameters.sexprs) {
                        vars.Remove(((SExpr.Atom)sexpr).Value.Lexeme);
                    }

                    return ret;
            }
        }
        else {

            CADRCell start = new CADRCell(left);
            CADRCell prev = start;

            foreach (SExpr s in list.sexprs) {
                CADRCell c = new CADRCell(evaluate(s));
                prev.Cdr = c;
                prev = c;
            }

            return start;
        }
        
    }

    private Object evaluate(SExpr.Atom atom)
    {
        Token value = atom.Value;

        if (value.Type == TokenType.NUMBER)
        {
            return value.Literal;
        }
        else
        {
            return resolve(value.Lexeme);
        }
    }
 

    private Object evaluate(SExpr.Null nully) {
        return nully;
    }

    private Object resolve(string lexeme) {
        if (vars.ContainsKey(lexeme) && !(vars[lexeme] is List<SExpr>)) { return vars[lexeme]; }
        
        return lexeme;
    }

       private Boolean eq(List<SExpr> list)
        {
            Object l = evaluate(list[0]);
            Object r = evaluate(list[1]);

            if (l is int && r is int)
            {
                return (int)l == (int)r;
            }
            else if (l is SExpr.Null && r is SExpr.Null)
            {
                return true;
            }
            else if (l is CADRCell && r is CADRCell)
            {
                return eqCarCdr((CADRCell)l, (CADRCell)r);
            }

            return false;
        }

    private Boolean eqCarCdr(CADRCell left, CADRCell right) {
        Boolean cont = false;

        if (left.Car is int && right.Car is int)
        {
            cont = ((int)left.Car == (int)right.Car);
        }
        else if (left.Car is SExpr.Null && right.Car is SExpr.Null)
        {
            cont = true;
        }
        else if (left.Car is CADRCell && right.Car is CADRCell)
        {
            cont = eqCarCdr((CADRCell)left.Car, (CADRCell)right.Car);
        }
        else { return false; }

        if (cont) {
            // cars match if we are here

            if (left.Cdr is int && right.Cdr is int)
            {
                return ((int)left.Cdr == (int)right.Cdr);
            }
            else if (left.Cdr is SExpr.Null && right.Cdr is SExpr.Null)
            {
                return true;
            }
            else if (left.Cdr is CADRCell && right.Cdr is CADRCell)
            {
                return eqCarCdr((CADRCell)left.Cdr, (CADRCell)right.Cdr);
            }
        }

        return false;
    }
    private void PrintList(SExpr.ListExpr list, StringBuilder result)
    {
        // Print the first value
        if (list.Left is SExpr.ListExpr innerListFirst)
        {
            // Recursively print nested lists
            result.Append("(");
            PrintList(innerListFirst, result);
            result.Append(")");
        }
        else
        {
            // Print atomic elements
            result.Append(list.Left);
        }

        // Print the rest of the list
        for (int i = 0; i < list.sexprs.Count; i++)
        {
            if (list.sexprs[i] is SExpr.ListExpr innerList)
            {
                // Recursively print nested lists
                result.Append(" (");
                PrintList(innerList, result);
                result.Append(")");
            }
            else
            {
                // Print atomic elements
                result.Append(" ");
                result.Append(list.sexprs[i]);
            }
        }
    }

  private Object define(List<SExpr> list)
{
    string func_name = ((SExpr.Atom)list[0]).Value.Lexeme;

    // Check if it's a variable assignment
    if (list.Count == 2)
    {
        // Variable assignment
        SExpr value = list[1];
        vars[func_name] = value;

        // Print the list
        StringBuilder result = new StringBuilder("(");
        if (value is SExpr.ListExpr listValue)
        {
            PrintList(listValue, result);
        }
        result.Append(")");

        return func_name + " = " + result.ToString();
    }
    else if (list.Count == 3)
    {
        // Function definition
        List<SExpr> func_info = new List<SExpr>();
        SExpr.ListExpr parameters = (SExpr.ListExpr)list[1];
        SExpr body = list[2];
        func_info.Add(parameters);
        func_info.Add(body);
        vars[func_name] = func_info;
        return "function \"" + func_name + "\" defined.";
    }
    else
    {
        return "Error: Invalid define statement for \"" + func_name + "\".";
    }
}


    // the symbol name is associated with the value of exp can return the name, or the value, or nil even
    private Object set(List<SExpr> list) {
        string varNamey = ((SExpr.Atom)list[0]).Value.Lexeme;
        Object value = evaluate(list[1]); // null, int, list, or cons cell

        vars[varNamey] = value;

        return "variable \"" + varNamey + "\" set.\n\t" + varNamey + " = " + value ; // not sure about this, feels right though
    }

    private Object cons(List<SExpr> list) {
        Object left = evaluate(list[0]);
        Object right = evaluate(list[1]);

        CADRCell c = new CADRCell(left, right);

        // Print the CADRCell
        //c.Print();

        return c;
    }

// Modify the 'car' function
    private Object car(List<SExpr> list) 
    {
        CADRCell c = (CADRCell)evaluate(list[0]);
        return c.Car;
    }

    private Object cdr(List<SExpr> list) {
        CADRCell c = (CADRCell)evaluate(list[0]);
        return c.Cdr;
    }





    private string printingList(List<SExpr> list) 
    {
        StringBuilder result = new StringBuilder("(");

        // Print each element in the list
        for (int i = 0; i < list.Count; i++) {
            result.Append(list[i]);

            // Append a space between elements (except for the last one)
            if (i < list.Count - 1) {
                result.Append(" ");
            }
        }

        result.Append(")");
        return result.ToString();
    }
// Modify the 'ify' 
private Object ify(List<SExpr> list)
{
    if (list.Count >= 3) // Ensure there are enough elements for condition, thenExpr, and elseExpr
    {
        for (int i = 0; i < list.Count - 1; i += 2)
        {
            if (truth(list[i]))
            {
                // If the test expression is true, evaluate and return the corresponding result expression
                if (i + 1 < list.Count)
                {
                    return evaluate(list[i + 1]);
                }
            }
        }

        // If no condition is true or the result is an empty list, return the else branch if available
        if (list.Count % 2 == 0)
        {
            return list[list.Count - 1];
        }
    }

    // If there are not enough elements or no condition is true, return a default value
    // You might want to adjust this based on your requirements
    return new SExpr.Null();
}

    // returns T if the expr is a name, () otherwise
    private Boolean symbol(SExpr sexpr) {
        if (sexpr is SExpr.Atom)
        {
            return ((SExpr.Atom)sexpr).Value.Type == TokenType.IDENTIFIER;
        }
        return false;
    }

    // returns T iff Expr is not an atom
    private Boolean list(SExpr sexpr) {
        Object eval = evaluate(sexpr);
        return !(eval is SExpr.Null || eval is int);
    }

    private bool truth(SExpr sexpr)
{
    Object eval = evaluate(sexpr);

    if (eval == null)
    {
        Console.WriteLine("Error: evaluate(sexpr) returned null.");
        return false;
    }

    // Consider any non-null, non-false value as true
    return !(eval is SExpr.Null || eval is false);
}


}
}

