#ifndef AST_H
#define AST_H

#include "../lexer/token.h"
#include "../lib/string.h"
#include "../lib/list.h"

typedef enum {
        LetStatement,
        ReturnStatement,
        ExpressionStatement,
        BlockStatement,
} ASTStatementType;

typedef enum {
        IdentifierExpression,
        BooleanExpression,
        IntegerExpression,
        StringExpression,
        ArrayExpression,
        PrefixExpression,
        InfixExpression,
        IfExpression,
        FunctionExpression,
        CallExpression,
        IndexExpression,
        HashMapExpression,
} ASTExpressionType;

typedef struct {
        List* statements;
} ASTProgram;

void print_ast_program(ASTProgram* program);

typedef struct {
        ASTStatementType type;
        void* value;
} ASTStatement;

typedef struct {
        ASTExpressionType type;
        void* value;
} ASTExpression;

typedef struct {
        ASTExpressionType type;
        HeapString value;
} ASTIdentifier;

void print_ast_identifier(ASTIdentifier* identifier);

typedef struct {
        ASTStatementType type;
        ASTIdentifier* name;
        ASTExpression* value;
} ASTLetStatement;

typedef struct {
        ASTStatementType type;
        ASTExpression* value;
} ASTReturnStatement;

typedef struct {
        ASTStatementType type;
        ASTExpression* expression;
} ASTExpressionStatement;

typedef struct {
        ASTStatementType type;
        List* statements;
} ASTBlockStatement;

typedef struct {
        ASTExpressionType type;
        int value;
} ASTBooleanExpression;

typedef struct {
        ASTExpressionType type;
        int value;
} ASTIntegerExpression;

typedef struct {
        ASTExpressionType type;
        HeapString value;
} ASTStringExpression;

typedef struct {
        ASTExpressionType type;
        List* value;
} ASTArrayExpression;

typedef struct {
        ASTExpressionType type;
        HeapString prefix;
        ASTExpression* right;
} ASTPrefixExpression;

typedef struct {
        ASTExpressionType type;
        HeapString infix;
        ASTExpression* left;
        ASTExpression* right;
} ASTInfixExpression;

typedef struct {
        ASTExpressionType type;
        ASTExpression* condition;
        ASTBlockStatement* consequence;
        ASTBlockStatement* alternative;
} ASTIfExpression;

typedef struct {
        ASTExpressionType type;
        List* parameters;
        ASTBlockStatement* body;
} ASTFunctionExpression;

typedef struct {
        ASTExpressionType type;
        ASTExpression* function;
        List* arguments;
} ASTCallExpression;

typedef struct {
        ASTExpressionType type;
        ASTExpression* left;
        ASTExpression* index;
} ASTIndexExpression;

#endif
