#include <stdio.h>
#include <string.h>

#include "src/lexer/lexer.h"
#include "src/lexer/token.h"
#include "src/parser/ast.h"
#include "src/parser/parser.h"

int
main(int argc, char** argv)
{
        FILE* file = fopen("test.mk", "r");
        Lexer* lexer = lexer_new(file);
        Parser* parser = parser_new(lexer);

        ASTProgram* program = parse_program(parser);
        print_ast_program(program);
        

        // while (1) {
        //         Token* token = lexer_get_token(lexer);
        //         token_print(token);
        //         if (token->type == Eof)
        //                 break;
        // }

        fclose(file);

        return 0;
}
