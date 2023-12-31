#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "string.h"

HeapString *
string_new(const char* text)
{
        HeapString * str = malloc(sizeof *str);
        str->pointer = malloc(4 * sizeof(char));
        strcat(str->pointer, text);
        str->allocated_length = 4;

        return str;
}

void
string_free(HeapString* string)
{
        free(string->pointer);
        free(string);
}

unsigned long
string_append(HeapString* string, const char* text)
{
        if ((strlen(string->pointer) + strlen(text)) <
            string->allocated_length) {
                strcat(string->pointer, text);
        } else {
                string->allocated_length *= 2;
                string->pointer = realloc(
                    string->pointer, string->allocated_length * sizeof(char));
                if (string->pointer == NULL) {
                        fprintf(stderr, "ERROR: realloc of string failed!");
                        return 0;
                }
                string_append(string, text);
        }
        return string->allocated_length;
}

void
string_print(HeapString* string)
{
        printf("String <%s, %ld>\n", string->pointer, string->allocated_length);
}
