#include <stdio.h>
#include <string.h>

#include "src/lexer/lexer.h"
#include "src/parser/ast.h"
#include "src/parser/parser.h"

int
main(int argc, char** argv)
{
        FILE* file = fopen("test.mk", "r");
        Lexer lexer = lexer_new(file);
        Parser parser = parser_new(&lexer);

        ASTProgram program = parse_program(&parser);

        fclose(file);

        return 0;
}
