#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>

#include "token.h"

typedef struct lexer
{
        FILE* file;
        int position;
        int read_position;
        char character;
} Lexer;

int is_space(char input);
int is_alpha(char input);
int is_numeric(char input);

void lexer_read_character(Lexer* lexer);
char lexer_peek_character(Lexer* lexer);
void lexer_read_string(Lexer* lexer);
Lexer lexer_new(FILE* file);
void lexer_print(Lexer* lexer);
Token lexer_get_token(Lexer* lexer);

#endif
