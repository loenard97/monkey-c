#include <stdio.h>

#include "ast.h"

void
print_ast_program(ASTProgram* program)
{
        printf("Program <>");
        list_print(program->statements);
}

void
print_ast_identifier(ASTIdentifier* identifier)
{
        printf("Identifier <%s>", identifier->value.pointer);
}
