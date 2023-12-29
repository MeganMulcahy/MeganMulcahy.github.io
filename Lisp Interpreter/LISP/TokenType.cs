namespace CraftingInterpreters.Lisp
{
    public enum TokenType
    {
        // Single-character tokens.
        LEFT_PAREN, RIGHT_PAREN,
        NULL,
        // Literals.
        IDENTIFIER, NUMBER, STRING,
        EOF
    }
}
