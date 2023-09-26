#ifndef PARSER_H
#define PARSER_H

#include "../lexer/lexer.h"
#include "../lexer/token.h"

typedef enum precedence {
        Lowest,
        Equals,
        Comparison,
        Sum,
        Product,
        Prefix,
        Call,
        Index,
} Precedence;

typedef struct parser {
        Lexer* lexer;
        Token* current_token;
        Token* peek_token;
} Parser;

void parser_next_token(Parser* parser);
int parser_expect_peek(Parser* parser, TokenType* token_type);

#endif
