#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

struct threadState {
    pthread_t id;
    int sum;
    const char* fileName;
};

void *getSumFromFile(void* threadState) {
    struct threadState *s = (struct threadState*)threadState;
    FILE *f = fopen(s -> fileName, "r");
    int num;
    while (fscanf(f, "%d", &num) != EOF)
    {
        s -> sum += num;
    }
    fclose(f);
    return NULL;
}

int main(int argc, char const* argv[]) {
    int totalSum = 0;
    struct threadState *threads = malloc((argc-1)*sizeof(struct threadState));
    // skip filename
    for (int i = 1; i < argc; i++)
    {
        threads[i-1].sum = 0;
        threads[i-1].fileName = argv[i];
        if (pthread_create(&threads[i-1].id, NULL, getSumFromFile,&threads[i-1]) != 0)
        {
            perror("Failed to create Thread");
            return EXIT_FAILURE;
        }
    }

    for (int i = 1; i < argc; i++)
    {
        pthread_join(threads[i-1].id, NULL);
        printf("sum %d = %d\n", i, threads[i-1].sum);
        totalSum += threads[i-1].sum;
    }
    
    printf("total sum = %d\n", totalSum);
    free(threads);
	return EXIT_SUCCESS;
}
