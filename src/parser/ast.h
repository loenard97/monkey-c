#ifndef AST_H
#define AST_H

#include "../lexer/token.h"
#include "../lib/string.h"
#include "../lib/list.h"

typedef enum {
        ProgramType,
        StatementType,
        ExpressionType,

        LetStatementType,
        ReturnStatementType,
        ExpressionStatementType,
        BlockStatementType,

        IdentifierExpressionType,
        BooleanExpressionType,
        IntegerExpressionType,
        StringExpressionType,
        ArrayExpressionType,
        PrefixExpressionType,
        InfixExpressionType,
        IfExpressionType,
        FunctionExpressionType,
        CallExpressionType,
        IndexExpressionType,
        HashMapExpressionType,
} ASTNodeType;

typedef struct {
        ASTNodeType type;
        void* node;
} ASTNode;

typedef struct {
        ASTNodeType type;
        void* statement;
} ASTStatement;

typedef struct {
        ASTNodeType type;
        void* expression;
} ASTExpression;

typedef struct {
        List* statements;
} ASTProgram;

void print_ast_program(ASTProgram* program);

typedef struct {
        ASTNodeType type;
        HeapString* value;
} ASTIdentifier;

void print_ast_identifier(ASTIdentifier* identifier);

typedef struct {
        ASTNodeType type;
        ASTIdentifier* identifier;
        ASTNode* expression;
} ASTLetStatement;

typedef struct {
        ASTNodeType type;
        ASTNode* expression;
} ASTReturnStatement;

typedef struct {
        ASTNodeType type;
        ASTNode* expression;
} ASTExpressionStatement;

typedef struct {
        ASTNodeType type;
        List* statements;
} ASTBlockStatement;

typedef struct {
        ASTNodeType type;
        int value;
} ASTBooleanExpression;

typedef struct {
        ASTNodeType type;
        int value;
} ASTIntegerExpression;

typedef struct {
        ASTNodeType type;
        HeapString* value;
} ASTStringExpression;

typedef struct {
        ASTNodeType type;
        List* value;
} ASTArrayExpression;

typedef struct {
        ASTNodeType type;
        HeapString* prefix;
        ASTExpression* right;
} ASTPrefixExpression;

typedef struct {
        ASTNodeType type;
        HeapString* infix;
        ASTExpression* left;
        ASTExpression* right;
} ASTInfixExpression;

typedef struct {
        ASTNodeType type;
        ASTExpression* condition;
        ASTBlockStatement* consequence;
        ASTBlockStatement* alternative;
} ASTIfExpression;

typedef struct {
        ASTNodeType type;
        List* parameters;
        ASTBlockStatement* body;
} ASTFunctionExpression;

typedef struct {
        ASTNodeType type;
        ASTExpression* function;
        List* arguments;
} ASTCallExpression;

typedef struct {
        ASTNodeType type;
        ASTExpression* left;
        ASTExpression* index;
} ASTIndexExpression;

#endif
