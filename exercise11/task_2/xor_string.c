#include <stdlib.h>
#include <string.h>

char *map_string(const char *input) {
    char *result = strdup(input);
    size_t len = strlen(result);
    for (size_t i = 0; i < len; i++) {
        char c = input[i];
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
            result[i] = c ^ 0x20;
        } else {
            result[i] = c;
        }
    }
    return result;
}
