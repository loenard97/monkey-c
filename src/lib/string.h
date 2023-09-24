#ifndef STRING_H
#define STRING_H

typedef struct heap_string {
        char* pointer;
        unsigned long allocated_length;
} HeapString;

unsigned long string_append(HeapString* string, const char* text);
void string_free(HeapString* string);
HeapString string_new(const char* text);
void string_print(HeapString* string);

#endif
