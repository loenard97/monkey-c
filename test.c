#include <stdio.h>

#include "src/libstring.h"

int
main() {
        HeapString str = string_new("first string");
        string_print(&str);

        string_append(&str, " hi ");
        string_print(&str);

        string_append(&str, " how are you? Is this getting really long or what?");
        string_print(&str);

        string_free(&str);
}
