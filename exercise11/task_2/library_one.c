#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *map_string(const char *input) {
    char *result = strdup(input);
    for (int i = 0; result[i]; i++) {
        if (islower(result[i]))
            result[i] = toupper(result[i]);
        else if (isupper(result[i]))
            result[i] = tolower(result[i]);
    }
    return result;
}
