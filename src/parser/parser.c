#include <stdio.h>
#include <string.h>

#include "ast.h"
#include "parser.h"
#include "../lexer/lexer.h"
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

ASTNode*
(*prefix_parse_fn(Parser* parser, Token* token))(Parser*)
{
        printf("prefix parse fn\n");
        switch (token->type) {
                case Identifier:
                        return parse_identifier;
                case Number:
                        return parse_integer_expression;
                case String:
                        return parse_string_expression;
                case Bang:
                case Minus:
                        return parse_prefix_expression;
                case Keyword:
                        if (0 == strcmp(token->literal->pointer, "true") || 0 == strcmp(token->literal->pointer, "false"))
                                return parse_boolean_expression;
                        if (0 == strcmp(token->literal->pointer, "if"))
                                return parse_if_expression;
                        if (0 == strcmp(token->literal->pointer, "fn"))
                                return parse_function_expression;
                case LParen:
                        return parse_grouped_expression;
                case LBracket:
                        return parse_array_expression;
                case LBrace:
                        return parse_hash_expression;
                default:
                        return NULL;
        }
}

ASTNode*
(*infix_parse_fn(Parser* parser, Token* token))(Parser*, ASTNode*)
{
        switch (token->type) {
                case Plus:
                case Minus:
                case Slash:
                case Astersik:
                case Equal:
                case NotEqual:
                case Lesser:
                case Greater:
                        return parse_infix_expression;
                case LParen:
                        return parse_call_expression;
                case LBracket:
                        return parse_index_expression;
                default:
                        return NULL;
        }
}

Parser *
parser_new(Lexer* lexer)
{
        printf("parser_new\n");
        Parser * parser = malloc(sizeof *parser);
        parser->lexer = lexer;

        Token * first_token = lexer_get_token(lexer);
        Token * second_token = lexer_get_token(lexer);
        printf("first_token = ");
        token_print(first_token);
        printf("\nsecond_token = ");
        token_print(second_token);
        printf("\n");
        parser->current_token = first_token;
        parser->peek_token = second_token;

        parser_print(parser);

        return parser;
}

void
parser_print(Parser* parser)
{
        printf("Parser <");
        token_print(parser->current_token);
        token_print(parser->peek_token);
        printf(">\n");
}

void
parser_next_token(Parser* parser)
{
        printf("parser next token... ");
        token_free(parser->current_token);
        parser->current_token = parser->peek_token;
        parser->peek_token = lexer_get_token(parser->lexer);
}


int
parser_expect_peek(Parser* parser, TokenType token_type)
{
        if (parser->peek_token->type == token_type) {
                parser_next_token(parser);
                return 1;
        }
        return 0;
}

ASTProgram*
parse_program(Parser* parser)
{
        printf("parse program\n");

        ASTProgram* program = malloc(sizeof *program);
        program->statements = list_new();

        ASTNode* next_node;
        while (Eof != parser->current_token->type) {
                printf("\tparse program loop\n");
                token_print(parser->current_token);
                token_print(parser->peek_token);
                printf("\n");
                next_node = parse_statement(parser);
                list_push(program->statements, next_node);
                token_print(parser->current_token);
                token_print(parser->peek_token);
                printf("\n");
        }

        return program;
}

ASTNode*
parse_identifier(Parser* parser)
{
        ASTIdentifier* identifier = malloc(sizeof *identifier);
        identifier->type = IdentifierExpressionType;
        identifier->value = parser->current_token->literal;

        ASTNode* node = malloc(sizeof *node);
        node->type = IdentifierExpressionType;
        node->node = identifier;

        return node;
}

ASTNode*
parse_statement(Parser* parser)
{
        printf("parse statement...\n");

        ASTStatement* statement = malloc(sizeof *statement);

        if (parser->current_token->type == Keyword) {
                if (strcmp(parser->current_token->literal->pointer, "let") == 0) {
                        statement->type = LetStatementType;
                        statement->statement = parse_let_statement(parser);
                }
                if (strcmp(parser->current_token->literal->pointer, "return") == 0) {
                        statement->type = ReturnStatementType;
                        statement->statement = parse_return_statement(parser);
                }
        }

        statement->type = ExpressionStatementType;
        statement->statement = parse_expression_statement(parser);

        ASTNode* node = malloc(sizeof *node);
        node->type = StatementType;
        node->node = statement;

        return node;
}

ASTNode*
parse_let_statement(Parser* parser)
{
        printf("parse let statement...\n");
        printf("parser->current_token = ");
        token_print(parser->current_token);
        printf("\n");

        ASTLetStatement* let_statement = malloc(sizeof *let_statement);
        let_statement->type = LetStatementType;

        parser_next_token(parser);
        
        let_statement->identifier = parse_identifier(parser);

        if (!parser_expect_peek(parser, Assign))
                printf("let statement has no =");

        parser_next_token(parser);

        let_statement->expression = parse_expression(parser, Lowest);

        if (parser->peek_token->type == Semicolon)
                parser_next_token(parser);

        ASTNode* node = malloc(sizeof *node);
        node->type = LetStatementType;
        node->node = let_statement;

        return node;
}

ASTNode*
parse_return_statement(Parser* parser)
{
        ASTReturnStatement* return_statement = malloc(sizeof *return_statement);
        return_statement->type = ReturnStatementType;

        parser_next_token(parser);

        return_statement->expression = parse_expression(parser, Lowest);
        
        if (parser->peek_token->type == Semicolon)
                parser_next_token(parser);

        ASTNode* node = malloc(sizeof *node);
        node->type = ReturnStatementType;
        node->node = return_statement;

        return node;
}

ASTNode*
parse_expression_statement(Parser* parser)
{
        printf("parse_expression_statement\n");
        ASTExpressionStatement* expression_statement = malloc(sizeof *expression_statement);
        expression_statement->type = ExpressionStatementType;

        expression_statement->expression = parse_expression(parser, Lowest);

        printf("parse state\n");
        token_print(parser->current_token);
        token_print(parser->peek_token);
        printf("\n");
        if (parser->current_token->type == Semicolon)
                parser_next_token(parser);

        ASTNode* node = malloc(sizeof *node);
        node->type = ExpressionStatementType;
        node->node = expression_statement;

        return node;
}

ASTNode*
parse_integer_expression(Parser* parser)
{
        printf("parse int expr\n");
        ASTIntegerExpression* integer_expression = malloc(sizeof *integer_expression);
        integer_expression->type = IntegerExpressionType;
        integer_expression->value = atoi(parser->current_token->literal->pointer);

        ASTNode* node = malloc(sizeof *node);
        node->type = IntegerExpressionType;
        node->node = integer_expression;

        printf("parse int expr done\n");

        return node;
}

ASTNode*
parse_string_expression(Parser* parser)
{
        ASTStringExpression* string_expression = malloc(sizeof *string_expression);
        string_expression->type = StringExpressionType;
        string_expression->value = parser->current_token->literal;

        ASTNode* node = malloc(sizeof *node);
        node->type = StringExpressionType;
        node->node = string_expression;

        return node;
}

ASTNode*
parse_prefix_expression(Parser* parser)
{
        ASTPrefixExpression* prefix_expression = malloc(sizeof *prefix_expression);
        prefix_expression->type = PrefixExpressionType;
        prefix_expression->prefix = parser->current_token->literal;

        parser_next_token(parser);

        prefix_expression->right = parse_expression(parser, Prefix);

        ASTNode* node = malloc(sizeof *node);
        node->type = PrefixExpressionType;
        node->node = prefix_expression;

        return node;
}

ASTNode*
parse_boolean_expression(Parser* parser)
{
        ASTBooleanExpression* boolean_expression = malloc(sizeof *boolean_expression);
        boolean_expression->type = BooleanExpressionType;

        if (strcmp(parser->current_token->literal->pointer, "true") == 0)
                boolean_expression->value = 1;
        else
                boolean_expression->value = 0;

        ASTNode* node = malloc(sizeof *node);
        node->type = BooleanExpressionType;
        node->node = boolean_expression;

        return node;
}

ASTNode*
parse_array_expression(Parser* parser)
{
        ASTArrayExpression* array_expression = malloc(sizeof *array_expression);
        array_expression->type = ArrayExpressionType;
        // List* elements = parse_expression_list(RBracket);
        // array_expression.value = elements;
        
        ASTNode* node = malloc(sizeof *node);
        node->type = ArrayExpressionType;
        node->node = array_expression;

        return node;
}

ASTNode*
parse_hash_expression(Parser* parser)
{
        ASTExpression* hash_expression = malloc(sizeof *hash_expression);

        ASTNode* node = malloc(sizeof *node);
        node->type = HashMapExpressionType;
        node->node = hash_expression;

        return node;
}

ASTNode*
parse_grouped_expression(Parser* parser)
{
        ASTExpression* grouped_expression = malloc(sizeof *grouped_expression);

        ASTNode* node = malloc(sizeof *node);
        node->type = ExpressionType;
        node->node = grouped_expression;

        return node;
}

ASTNode*
parse_if_expression(Parser* parser)
{
        ASTIfExpression* if_expression = malloc(sizeof *if_expression);
        if_expression->type = IfExpressionType;

        parser_next_token(parser);
        parser_next_token(parser);

        if_expression->condition = parse_expression(parser, Lowest);

        parser_next_token(parser);
        parser_next_token(parser);

        if_expression->consequence = parse_block_statement(parser);

        if (parser->peek_token->type == Keyword && strcmp(parser->peek_token->literal->pointer, "else") == 0) {
                parser_next_token(parser);
                parser_next_token(parser);

                if_expression->alternative = parse_block_statement(parser);
        }

        ASTNode* node = malloc(sizeof *node);
        node->type = IfExpressionType;
        node->node = if_expression;

        return node;
}

ASTNode*
parse_function_expression(Parser* parser)
{
        ASTFunctionExpression* function_expression = malloc(sizeof *function_expression);
        function_expression->type = FunctionExpressionType;

        parser_next_token(parser);

        function_expression->parameters = parse_function_parameters(parser);

        parser_next_token(parser);

        function_expression->body = parse_block_statement(parser);

        ASTNode* node = malloc(sizeof *node);
        node->type = FunctionExpressionType;
        node->node = function_expression;

        return node;
}

List*
parse_function_parameters(Parser* parser)
{
        parser_next_token(parser);
        parser_next_token(parser);

        List* identifier = malloc(sizeof *identifier);
        return identifier;
}

ASTNode*
parse_infix_expression(Parser* parser, ASTNode* left)
{
        ASTInfixExpression* infix_expression = malloc(sizeof *infix_expression);
        infix_expression->type = InfixExpressionType;

        Precedence precedence = precedence_from_token(parser->current_token);

        infix_expression->right = parse_expression(parser, precedence);

        infix_expression->infix = malloc(sizeof *infix_expression->infix);
        infix_expression->infix->pointer = parser->current_token->literal->pointer;

        infix_expression->left = parse_expression(parser, precedence);

        ASTNode* node = malloc(sizeof *node);
        node->type = InfixExpressionType;
        node->node = infix_expression;

        return node;
}

ASTNode*
parse_call_expression(Parser* parser, ASTNode* function)
{
        ASTCallExpression* call_expression = malloc(sizeof *call_expression);
        return call_expression;
}

List*
parse_call_arguments(Parser* parser)
{
        List* arguments = malloc(sizeof *arguments);
        return arguments;
}

ASTNode*
parse_index_expression(Parser* parser, ASTNode* left)
{
        ASTExpression* index_expression = malloc(sizeof *index_expression);
        return index_expression;
}

ASTNode*
parse_block_statement(Parser* parser)
{
        ASTBlockStatement* bs = malloc(sizeof *bs);
        return bs;
}

ASTNode*
parse_expression(Parser* parser, Precedence precedence)
{
        printf("parse expression\n");
        ASTNode* (*prefix_function)(Parser*) = prefix_parse_fn(parser, parser->current_token);

        if (NULL == prefix_function) {
                printf("prefix func null\n");
                return NULL;
        }
        printf("prefix parse func\n");
        printf("func %p\n", prefix_function);
        ASTNode* left = malloc(sizeof *left);
        left->node = prefix_function(parser);
        printf("called prefix parse func\n");

        printf("precedence\n");
        Precedence peek_precedence = precedence_from_token(parser->peek_token);

        while (parser->peek_token->type != Semicolon && precedence < peek_precedence) {
                printf("while\n");
               ASTNode* (*infix_function)(Parser*, ASTNode*) = infix_parse_fn(parser, parser->peek_token);
               if (NULL == infix_function)
                       return left;

               parser_next_token(parser);
               left = infix_function(parser, left);
        }

        return left;
}
