#include <stdio.h>
#include <string.h>

#include "token.h"
#include "lexer.h"

int
is_space(char input)
{
        switch (input) {
                case ' ':
                        return 1;
                case '\n':
                        return 1;
                default:
                        return 0;
        }
}

int
is_alpha(char input)
{
        if (
                input >= 'A' && input <= 'Z' 
                || input >= 'a' && input <= 'z' 
                || input == '_'
                || input == '-'
        ) {
                return 1;
        }
        return 0;
}

int
is_numeric(char input)
{
        if (input >= '0' && input <= '9' || input == '.') {
                return 1;
        }
        return 0;
}

int
is_keyword(char* word)
{
        int n_keywords = 7;
        const char* keywords[n_keywords];
        keywords[0] = "fn";
        keywords[1] = "let";
        keywords[2] = "true";
        keywords[3] = "false";
        keywords[4] = "if";
        keywords[5] = "else";
        keywords[6] = "return";

        for (int i = 0; i < n_keywords; ++i) {
                if (strcmp(word, keywords[i]) == 0) {
                        return 1;
                }
        }
        return 0;
}

/* Advance lexer by one character */
void
lexer_read_character(Lexer* lexer)
{
        lexer->character = fgetc(lexer->file);
        lexer->position = lexer->read_position;
        lexer->read_position++;
}

/* Peek next character without advancing */
char
lexer_peek_character(Lexer* lexer)
{
        char c = fgetc(lexer->file);
        if (c != EOF)
                fseek(lexer->file, -1, SEEK_CUR);
        return c;
}

/* Init new Lexer */
Lexer
lexer_new(FILE* file)
{
        Lexer lexer;
        lexer.file = file;
        lexer.position = 0;
        lexer.read_position = 0;
        lexer.character = '\0';

        lexer_read_character(&lexer);

        return lexer;
}

/* Print Lexer */
void
lexer_print(Lexer* lexer)
{
        printf(
            "Lexer <%d, %d, %c>\n",
            lexer->position,
            lexer->read_position,
            lexer->character);
}

/* Return next token */
Token
lexer_get_token(Lexer* lexer)
{
        Token token;

        while (is_space(lexer->character)) {
                lexer_read_character(lexer);
        }

        switch (lexer->character) {
                case '=':
                        if (lexer_peek_character(lexer) == '=') {
                                lexer_read_character(lexer);
                                token = token_new(Equal, "==");
                        } else {
                                token = token_new(Assign, "=");
                        }
                        break;
                case '+':
                        token = token_new(Plus, "+");
                        break;
                case '-':
                        token = token_new(Minus, "-");
                        break;
                case '!':
                        if (lexer_peek_character(lexer) == '=') {
                                lexer_read_character(lexer);
                                token = token_new(NotEqual, "!=");
                        } else {
                                token = token_new(Bang, "!");
                        }
                        break;
                case '*':
                        token = token_new(Astersik, "*");
                        break;
                case '/':
                        token = token_new(Slash, "/");
                        break;
                case '<':
                        token = token_new(Lesser, "<");
                        break;
                case '>':
                        token = token_new(Greater, ">");
                        break;

                case ',':
                        token = token_new(Comma, ",");
                        break;
                case ';':
                        token = token_new(Semicolon, ";");
                        break;
                case ':':
                        token = token_new(Colon, ":");
                        break;
                case '"':
                        lexer_read_character(lexer);
                        HeapString str = string_new(&lexer->character);
                        while (lexer_peek_character(lexer) != '"') {
                                lexer_read_character(lexer);
                                string_append(&str, &lexer->character);
                        }
                        lexer_read_character(lexer);
                        lexer_read_character(lexer);
                        token.type = String;
                        token.literal = str;

                        return token;
                case '(':
                        token = token_new(LParen, "(");
                        break;
                case ')':
                        token = token_new(RParen, ")");
                        break;
                case '{':
                        token = token_new(LBrace, "{");
                        break;
                case '}':
                        token = token_new(RBrace, "}");
                        break;
                case '[':
                        token = token_new(LBracket, "[");
                        break;
                case ']':
                        token = token_new(RBracket, "]");
                        break;

                case EOF:
                        token = token_new(Eof, "Eof");
                        return token;

                default:
                        if (is_alpha(lexer->character)) {
                                HeapString word = string_new("");
                                while (is_alpha(lexer->character)) {
                                        string_append(&word, &lexer->character);
                                        lexer_read_character(lexer);
                                }
                                if (is_keyword(word.pointer))
                                        token.type = Keyword;
                                else
                                        token.type = Identifier;
                                token.literal = word;
                        } else if (is_numeric(lexer->character)) {
                                HeapString number = string_new("");
                                while (is_numeric(lexer->character)) {
                                        string_append(&number, &lexer->character);
                                        lexer_read_character(lexer);
                                }
                                token.type = Number;
                                token.literal = number;
                        }
                        return token;
        }

        lexer_read_character(lexer);
        return token;
}
