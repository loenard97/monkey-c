#include <stdio.h>

#include "ast.h"

void
print_ast_program(ASTProgram* program)
{
        printf("Program <");
        list_print(program->statements);
        printf(">\n");
}

void
print_ast_identifier(ASTIdentifier* identifier)
{
        printf("Identifier <%s>", identifier->value->pointer);
}

void
print_node(ASTNode* node)
{
        switch (node->type) {
                case IntegerExpressionType:
                        // printf("IntegerNode %s\n", node->node->value);
        }
}
