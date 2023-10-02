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

void*
prefix_parse_fn(Parser* parser, Token* token)
{
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
                        if (0 == strcmp(token->literal.pointer, "true") || 0 == strcmp(token->literal.pointer, "false"))
                                return parse_boolean_expression;
                        if (0 == strcmp(token->literal.pointer, "if"))
                                return parse_if_expression;
                        if (0 == strcmp(token->literal.pointer, "fn"))
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

void*
infix_parse_fn(Parser* parser, Token* token)
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

Parser
parser_new(Lexer* lexer)
{
        Parser parser;
        parser.lexer = lexer;

        Token first_token = lexer_get_token(lexer);
        Token second_token = lexer_get_token(lexer);
        parser.current_token = &first_token;
        parser.peek_token = &second_token;

        return parser;
}

void
parser_next_token(Parser* parser)
{
        printf("parser next token... ");

        if (NULL != parser->peek_token)
                *parser->current_token = *parser->peek_token;
        Token next_token = lexer_get_token(parser->lexer);
        parser->peek_token = &next_token;

        printf("next_token = ");
        token_print(&next_token);
        printf("\n");

        printf("new state:\n");
        printf("cur tok %p\n", parser->current_token);
        if (NULL != parser->current_token) {
                printf("\tparser->current_token = ");
                token_print(parser->current_token);
                printf("\n");
        }
        printf("\tparser->peek_token = ");
        token_print(parser->peek_token);
        printf("\n");

        printf("parser next token done\n\n");
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

ASTProgram
parse_program(Parser* parser)
{
        printf("parse program\n");
        printf("\tparser->current_token = ");
        token_print(parser->current_token);
        printf("\n");

        ASTProgram program;
        ASTStatement next_node;
        List statements;

        while (NULL != parser->current_token) {
                next_node = parse_statement(parser);
                list_push(&statements, &next_node);
        }

        program.statements = &statements;
        return program;
}

ASTIdentifier
parse_identifier(Parser* parser)
{
        ASTIdentifier identifier;
        identifier.type = IdentifierExpression;
        identifier.value = parser->current_token->literal;

        return identifier;
}

ASTStatement
parse_statement(Parser* parser)
{
        printf("parse statement...\n");
        printf("parser->current_token = ");
        token_print(parser->current_token);
        printf("\n");

        ASTStatement statement;

        if (parser->current_token->type == Keyword) {
                if (strcmp(parser->current_token->literal.pointer, "let") == 0) {
                        statement.type = LetStatement;
                        ASTLetStatement let_statement = parse_let_statement(parser);
                        statement.value = &let_statement;
                }
                if (strcmp(parser->current_token->literal.pointer, "return") == 0) {
                        statement.type = ReturnStatement;
                        ASTReturnStatement return_statement = parse_return_statement(parser);
                        statement.value = &return_statement;
                }
        }

        statement.type = ExpressionStatement;
        ASTExpressionStatement expression_statement = parse_expression_statement(parser);
        statement.value = &expression_statement;

        return statement;
}

ASTLetStatement
parse_let_statement(Parser* parser)
{
        printf("parse let statement...\n");
        printf("parser->current_token = ");
        token_print(parser->current_token);
        printf("\n");

        parser_next_token(parser);
        
        ASTIdentifier identifier;
        identifier.type = IdentifierExpression;
        identifier.value = parser->current_token->literal;

        printf("let_statement.identifier = ");
        print_ast_identifier(&identifier);
        printf("\n");

        if (!parser_expect_peek(parser, Assign))
                printf("let statement has no =");

        parser_next_token(parser);

        ASTExpression expression = parse_expression(parser, Lowest);

        if (parser->peek_token->type == Semicolon)
                parser_next_token(parser);

        ASTLetStatement let_statement;
        let_statement.type = LetStatement;
        let_statement.name = &identifier;
        let_statement.value = &expression;

        return let_statement;
}

ASTReturnStatement
parse_return_statement(Parser* parser)
{
        parser_next_token(parser);

        ASTExpression expression = parse_expression(parser, Lowest);
        
        if (parser->peek_token->type == Semicolon)
                parser_next_token(parser);

        ASTReturnStatement return_statement;
        return_statement.type = ReturnStatement;
        return_statement.value = &expression;

        return return_statement;
}

ASTExpressionStatement
parse_expression_statement(Parser* parser)
{
        ASTExpression expression = parse_expression(parser, Lowest);

        if (parser->peek_token->type == Semicolon)
                parser_next_token(parser);

        ASTExpressionStatement expression_statement;
        expression_statement.type = ExpressionStatement;
        expression_statement.expression = &expression;

        return expression_statement;
}

ASTIntegerExpression
parse_integer_expression(Parser* parser)
{
        ASTIntegerExpression integer_expression;
        integer_expression.type = IntegerExpression;
        integer_expression.value = atoi(parser->current_token->literal.pointer);

        return integer_expression;
}

ASTStringExpression
parse_string_expression(Parser* parser)
{
        ASTStringExpression string_expression;
        string_expression.type = StringExpression;
        string_expression.value = parser->current_token->literal;

        return string_expression;
}

ASTPrefixExpression
parse_prefix_expression(Parser* parser)
{
        ASTPrefixExpression prefix_expression;
        prefix_expression.type = PrefixExpression;
        prefix_expression.prefix = parser->current_token->literal;

        parser_next_token(parser);

        ASTExpression expression = parse_expression(parser, Prefix);
        prefix_expression.right = &expression;

        return prefix_expression;
}

ASTBooleanExpression
parse_boolean_expression(Parser* parser)
{
        ASTBooleanExpression boolean_expression;
        boolean_expression.type = BooleanExpression;

        if (strcmp(parser->current_token->literal.pointer, "true") == 0)
                boolean_expression.value = 1;
        else
                boolean_expression.value = 0;

        return boolean_expression;
}

ASTArrayExpression
parse_array_expression(Parser* parser)
{
        ASTArrayExpression array_expression;
        array_expression.type = ArrayExpression;
        // List* elements = parse_expression_list(RBracket);
        // array_expression.value = elements;

        return array_expression;
}

ASTExpression
parse_hash_expression(Parser* parser)
{
        ASTExpression hash_expression;
        return hash_expression;
}

ASTExpression
parse_grouped_expression(Parser* parser)
{
        ASTExpression grouped_expression;
        return grouped_expression;
}

ASTIfExpression
parse_if_expression(Parser* parser)
{
        ASTIfExpression if_expression;

        parser_next_token(parser);
        parser_next_token(parser);

        ASTExpression condition = parse_expression(parser, Lowest);

        parser_next_token(parser);
        parser_next_token(parser);

        ASTBlockStatement consequence = parse_block_statement(parser);
        ASTBlockStatement alternative;

        if (parser->peek_token->type == Keyword && strcmp(parser->peek_token->literal.pointer, "else") == 0) {
                parser_next_token(parser);
                parser_next_token(parser);

                ASTBlockStatement alternative = parse_block_statement(parser);
        }

        if_expression.type = IfExpression;
        if_expression.condition = &condition;
        if_expression.consequence = &consequence;
        if_expression.alternative = &alternative;

        return if_expression;
}

ASTFunctionExpression
parse_function_expression(Parser* parser)
{
        parser_next_token(parser);

        List parameters = parse_function_parameters(parser);

        parser_next_token(parser);

        ASTBlockStatement body = parse_block_statement(parser);

        ASTFunctionExpression function_expression;
        function_expression.type = FunctionExpression;
        function_expression.parameters = &parameters;
        function_expression.body = &body;

        return function_expression;
}

List
parse_function_parameters(Parser* parser)
{
        parser_next_token(parser);
        parser_next_token(parser);

        List identifier;
        return identifier;
}

ASTInfixExpression
parse_infix_expression(Parser* parser, ASTExpression* left)
{
        Precedence precedence = precedence_from_token(parser->current_token);

        ASTExpression right = parse_expression(parser, precedence);

        ASTInfixExpression infix_expression;
        infix_expression.type = InfixExpression;
        infix_expression.right = &right;
        infix_expression.infix = parser->current_token->literal;
        infix_expression.left = left;

        return infix_expression;
}

ASTCallExpression
parse_call_expression(Parser* parser, ASTExpression* function)
{
        ASTCallExpression call_expression;
        return call_expression;
}

List
parse_call_arguments(Parser* parser)
{
        List arguments;
        return arguments;
}

ASTExpression
parse_index_expression(Parser* parser, ASTExpression* left)
{
        ASTExpression index_expression;
        return index_expression;
}

ASTBlockStatement
parse_block_statement(Parser* parser)
{
        ASTBlockStatement block_statement;
        return block_statement;
}

ASTExpression
parse_expression(Parser* parser, Precedence precedence)
{
        
}
