#include <stdlib.h>
#include <string.h>
#include <time.h>

char *map_string(const char *input) {
    srand(time(NULL));
    size_t len = strlen(input);
    char *result = malloc(len + 6); // +5 random + null
    strcpy(result, input);
    for (int i = 0; i < 5; i++) {
        result[len + i] = 'A' + rand() % 26;
    }
    result[len + 5] = '\0';
    return result;
}
