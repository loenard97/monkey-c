#include <stdio.h>

#include "../lib/string.h"
#include "token.h"

Token
token_new(TokenType type, const char* literal)
{
        Token token;
        token.type = type;
        token.literal = string_new(literal);

        return token;
}

void
token_free(Token* token)
{
        string_free(&token->literal);
}

void
token_print(Token* token)
{
        const char* token_names[] = {
                "Identifier", "Number",       "String", "Keyword",
                "Eof",        "Illegal",

                "Assign",     "Plus",         "Minus",  "Bang",
                "Asterisk",   "Slash",        "Lesser", "LesserEqual",
                "Greater",    "GreaterEqual", "Equal",  "NotEqual",

                "Comma",      "Semicolon",    "Colon",  "LParen",
                "RParen",     "LBrace",       "RBrace", "LBracket",
                "RBracket",
        };

        printf(
            "Token <%s, '%s'>\n",
            token_names[token->type],
            token->literal.pointer);
}
