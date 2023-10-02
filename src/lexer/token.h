#ifndef TOKEN_H
#define TOKEN_H

#include "../lib/string.h"

typedef enum token_type
{
        Identifier,
        Number,
        String,
        Keyword,
        Eof,
        Illegal,

        Assign,
        Plus,
        Minus,
        Bang,
        Astersik,
        Slash,
        Lesser,
        LesserEqual,
        Greater,
        GreaterEqual,
        Equal,
        NotEqual,

        Comma,
        Semicolon,
        Colon,
        LParen,
        RParen,
        LBrace,
        RBrace,
        LBracket,
        RBracket,
} TokenType;

typedef struct token
{
        TokenType type;
        HeapString * literal;
} Token;

Token * token_new(TokenType type, const char* literal);
void token_free(Token* token);
void token_print(Token* token);

#endif
