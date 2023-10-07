#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
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

Precedence precedence_from_token(Token* token);

Parser * parser_new(Lexer* lexer);
void parser_print(Parser* parser);
void parser_next_token(Parser* parser);
int parser_expect_peek(Parser* parser, TokenType token_type);

ASTNode* (*prefix_parse_fn(Parser* parser, Token* token))(Parser*);
ASTNode* (*infix_parse_fn(Parser* parser, Token* token))(Parser*, ASTNode*);

ASTProgram* parse_program(Parser* parser);
ASTNode* parse_identifier(Parser* parser);
ASTNode* parse_statement(Parser* parser);
ASTNode* parse_let_statement(Parser* parser);
ASTNode* parse_return_statement(Parser* parser);
ASTNode* parse_expression_statement(Parser* parser);
ASTNode* parse_integer_expression(Parser* parser);
ASTNode* parse_string_expression(Parser* parser);
ASTNode* parse_prefix_expression(Parser* parser);
ASTNode* parse_boolean_expression(Parser* parser);
ASTNode* parse_array_expression(Parser* parser);
ASTNode* parse_hash_expression(Parser* parser);
ASTNode* parse_grouped_expression(Parser* parser);
ASTNode* parse_if_expression(Parser* parser);
ASTNode* parse_function_expression(Parser* parser);
List* parse_function_parameters(Parser* parser);
ASTNode* parse_infix_expression(Parser* parser, ASTNode* left);
ASTNode* parse_call_expression(Parser* parser, ASTNode* function);
List* parse_call_arguments(Parser* parser);
ASTNode* parse_index_expression(Parser* parser, ASTNode* left);
ASTNode* parse_block_statement(Parser* parser);
ASTNode* parse_expression(Parser* parser, Precedence precedence);

#endif
