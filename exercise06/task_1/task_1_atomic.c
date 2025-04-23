#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <stdio.h>
#include <stdatomic.h>

#define THREADS 500
#define RUNS 50000

void *updateCounter(void* arg) {
    atomic_int *counter = (atomic_int*)arg;
    for (size_t i = 0; i < RUNS; i++)
    {
        if (i%2 == 0)
        {
            *counter += 73;
        } else {
            *counter -= 71;
        }
    }
    return NULL;
}

int main()
{
    atomic_int counter = 0;
    pthread_t threads[THREADS];
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
    return EXIT_SUCCESS;
}
