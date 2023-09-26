#ifndef AST_H
#define AST_H

#include "../lexer/token.h"
#include "../lib/string.h"
#include "../lib/list.h"

typedef enum expression_type {
        TypeIdentifier,
} ExpressionType;

typedef struct expression {
        ExpressionType type;
        void* value;
} MExpression;

typedef struct program {
        Token* token;
        List* statements;
} Program;

typedef struct identifier {
        Token* token;
        HeapString* value;
} MIdentifier;

typedef struct let_statement {
        Token* token;
        MIdentifier* name;
        MExpression* value;
} MLetStatement;

#endif
