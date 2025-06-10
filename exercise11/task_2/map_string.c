#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char *(*plugin_func)(const char *);

int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <string> <plugin1.so> [plugin2.so ...]\n", argv[0]);
        return 1;
    }

    char *current = strdup(argv[1]);
    if (!current) {
        perror("strdup");
        return 1;
    }

    for (int i = 2; i < argc; i++) {
        void *handle = dlopen(argv[i], RTLD_NOW);
        if (!handle) {
            fprintf(stderr, "Failed to open %s: %s\n", argv[i], dlerror());
            free(current);
            return 1;
        }

        dlerror();
        plugin_func func = (plugin_func)dlsym(handle, "map_string");
        char *error = dlerror();
        if (error != NULL) {
            fprintf(stderr, "Failed to find map_string in %s: %s\n", argv[i], error);
            dlclose(handle);
            free(current);
            return 1;
        }

        char *result = func(current);
        printf("%s: %s\n", argv[i], result);

        free(current);
        current = result;

        dlclose(handle);
    }

    free(current);
    return 0;
}
