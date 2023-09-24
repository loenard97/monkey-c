#include <stdio.h>
#include <string.h>

#include "src/lexer/lexer.h"

int
main(int argc, char** argv)
{
        FILE* file = fopen("monkey_examples/simple.mk", "r");
        Lexer lexer = lexer_new(file);

        Token token;
        while (1) {
                token = lexer_get_token(&lexer);
                if (token.type == Eof) {
                        break;
                }
                token_print(&token);
                // token_free(&token);
        }

        fclose(file);

        return 0;
}
