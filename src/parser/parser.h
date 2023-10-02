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
void parser_next_token(Parser* parser);
int parser_expect_peek(Parser* parser, TokenType token_type);

void* prefix_parse_fn(Parser* parser, Token* token);
void* infix_parse_fn(Parser* parser, Token* token);

ASTProgram parse_program(Parser* parser);
ASTIdentifier parse_identifier(Parser* parser);
ASTStatement parse_statement(Parser* parser);
ASTLetStatement parse_let_statement(Parser* parser);
ASTReturnStatement parse_return_statement(Parser* parser);
ASTExpressionStatement parse_expression_statement(Parser* parser);
ASTIntegerExpression parse_integer_expression(Parser* parser);
ASTStringExpression parse_string_expression(Parser* parser);
ASTPrefixExpression parse_prefix_expression(Parser* parser);
ASTBooleanExpression parse_boolean_expression(Parser* parser);
ASTArrayExpression parse_array_expression(Parser* parser);
ASTExpression parse_hash_expression(Parser* parser);
ASTExpression parse_grouped_expression(Parser* parser);
ASTIfExpression parse_if_expression(Parser* parser);
ASTFunctionExpression parse_function_expression(Parser* parser);
List parse_function_parameters(Parser* parser);
ASTInfixExpression parse_infix_expression(Parser* parser, ASTExpression* left);
ASTCallExpression parse_call_expression(Parser* parser, ASTExpression* function);
List parse_call_arguments(Parser* parser);
ASTExpression parse_index_expression(Parser* parser, ASTExpression* left);
ASTBlockStatement parse_block_statement(Parser* parser);
ASTExpression parse_expression(Parser* parser, Precedence precedence);

#endif
