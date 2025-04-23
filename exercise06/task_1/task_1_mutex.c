#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <stdio.h>

#define THREADS 500
#define RUNS 50000

pthread_mutex_t lockCounter;

void *updateCounter(void* arg) {
    int *counter = (int*)arg;
    for (size_t i = 0; i < RUNS; i++)
    {
        if (pthread_mutex_lock(&lockCounter) != 0)
        {
            exit(EXIT_FAILURE);
        }
        if (i%2 == 0)
        {
            *counter += 73;
        } else {
            *counter -= 71;
        }
        if (pthread_mutex_unlock(&lockCounter) != 0) {
            exit(EXIT_FAILURE);
        }
    }
    return NULL;
}

int main()
{
    int counter = 0;
    pthread_t threads[THREADS];
    if (pthread_mutex_init(&lockCounter, NULL) != 0)
    {
        perror("pthread_mutex_init");
        return EXIT_FAILURE;
    }
    
    for (size_t i = 0; i < THREADS; i++)
    {
        if (pthread_create(&threads[i], NULL, updateCounter, &counter) != 0)
        {
            perror("pthread_create");
            return EXIT_FAILURE;
        }
    }
    
    for (size_t i = 0; i < THREADS; i++)
    {
        if (pthread_join(threads[i], NULL) != 0)
        {
            perror("pthread_join");
            return EXIT_FAILURE;
        }
        
    }
    
    printf("%d\n", counter);
    pthread_mutex_destroy(&lockCounter);
    return EXIT_SUCCESS;
}
