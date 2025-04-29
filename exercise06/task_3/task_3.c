#include "myqueue.h"
#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

pthread_mutex_t lockQueue;

pthread_cond_t elementsAvailable = PTHREAD_COND_INITIALIZER;

struct consumerState
{
    myqueue *elements;
    int id;
};


void* accumulate(void* args) {
	int* localSum = malloc(sizeof(int));
	*localSum = 0;
	struct consumerState* state = (struct consumerState*)args;

	while (true) {
        pthread_mutex_lock(&lockQueue);

        while (myqueue_is_empty(state->elements)) {
            pthread_cond_wait(&elementsAvailable, &lockQueue);
        }
        int element = myqueue_pop(state->elements);
        pthread_mutex_unlock(&lockQueue);

        if (element == INT_MAX)
        {
            break;
        }
        
		*localSum += element;
	}
    printf("Consumer %d sum: %d\n", state->id, *localSum);
    free(state);
	return localSum;
}

int main(int argc, char const* argv[]) {
	if (argc != 3) {
		printf("Usage: %s <c> <n>\n", argv[0]);
		return EXIT_FAILURE;
	}
	int consumers = atoi(argv[1]);
	int elements = atoi(argv[2]);

	myqueue eQueue;
	myqueue_init(&eQueue);

	if (pthread_mutex_init(&lockQueue, NULL) != 0) {
		perror("pthread_mutex_init");
		return EXIT_FAILURE;
	}

    // spawn consumers
	pthread_t *cThreads = malloc(sizeof(pthread_t) * consumers);
	for (int i = 0; i < consumers; i++) {
        struct consumerState *s = malloc(sizeof(struct consumerState));
        s->elements = &eQueue;
        s->id = i;
		if (pthread_create(&cThreads[i], NULL, accumulate, s) != 0) {
            free(s);
			perror("pthread_create");
			return EXIT_FAILURE;
		}
	}

    // feed
    for (int i = 1; i <= elements; i++)
    {
        pthread_mutex_lock(&lockQueue);
        myqueue_push(&eQueue, i % 2 ? i : -i);
        pthread_cond_signal(&elementsAvailable);
        pthread_mutex_unlock(&lockQueue);
    }

    // feed stoppers
    for (int i = 0; i < consumers; i++)
    {
        pthread_mutex_lock(&lockQueue);
        myqueue_push(&eQueue, INT_MAX);
        pthread_cond_signal(&elementsAvailable);
        pthread_mutex_unlock(&lockQueue);
    }

    // accumulate results
    int sum = 0;
    for (int i = 0; i < consumers; i++)
    {
        int *localSum = NULL;
        if (pthread_join(cThreads[i], (void**)&localSum) != 0)
        {
            perror("pthread_join");
            free(localSum);
            return EXIT_FAILURE;
        }
        sum += *localSum;
        free(localSum);
    }
    
    pthread_mutex_destroy(&lockQueue);
    pthread_cond_destroy(&elementsAvailable);
    free(cThreads);
    printf("Final sum: %d\n", sum);
	return EXIT_SUCCESS;
}
