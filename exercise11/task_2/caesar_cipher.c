#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *map_string(const char *input) {
    char *result = strdup(input);
    size_t len = strlen(result);
    for (int i = 0; i<len; i++) {
        char c = input[i];
        if ('A' <= c && c <= 'Z') {
            result[i] = ((c - 'A' + 13) % 26) + 'A';
        } else if ('a' <= c && c <= 'z') {
            result[i] = ((c - 'a' + 13) % 26) + 'a';
        } else {
            result[i] = c;
        }
    }
    return result;
}
