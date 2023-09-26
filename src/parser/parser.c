#include "parser.h"
#include "../lexer/token.h"

Precedence
precedence_from_token(Token* token)
{
        switch (token->type)
        {
                case Equal:
                case NotEqual:
                        return Equals;
                case Lesser:
                case Greater:
                        return Comparison;
                case Plus:
                case Minus:
                        return Sum;
                case Astersik:
                case Slash:
                        return Product;
                case LParen:
                        return Call;
                case LBracket:
                        return Index;
                default:
                        return Lowest;
        }
}
