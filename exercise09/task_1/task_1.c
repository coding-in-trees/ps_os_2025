#include "myqueue.h"
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_ITEMS 1e7

#if USE_MY_MUTEX
	#include <sched.h>
	#include <stdatomic.h>

typedef struct {
	atomic_flag flag;
} my_mutex_t;

static void my_mutex_init(my_mutex_t* mtx) {
	;
	atomic_flag_clear(&mtx->flag);
}

static void my_mutex_lock(my_mutex_t* mtx) {
	while (atomic_flag_test_and_set(&mtx->flag)) {
		sched_yield();
	}
}

static void my_mutex_unlock(my_mutex_t* mtx) {
	atomic_flag_clear(&mtx->flag);
}
	#define MUTEX_TYPE my_mutex_t
	#define MUTEX_INIT(m) my_mutex_init(&(m))
	#define MUTEX_LOCK(m) my_mutex_lock(&(m))
	#define MUTEX_UNLOCK(m) my_mutex_unlock(&(m))
#else
	#define MUTEX_TYPE pthread_mutex_t
	#define MUTEX_INIT(m) pthread_mutex_init(&(m), NULL)
	#define MUTEX_LOCK(m) pthread_mutex_lock(&(m))
	#define MUTEX_UNLOCK(m) pthread_mutex_unlock(&(m))
#endif

struct shared_t {
	myqueue queue;
	MUTEX_TYPE mutex;
};

void* producer(void* arg) {
	struct shared_t* shared_data = (struct shared_t*)arg;
	for (int i = 0; i < NUM_ITEMS; ++i) {
		MUTEX_LOCK(shared_data->mutex);
		myqueue_push(&shared_data->queue, 1);
		MUTEX_UNLOCK(shared_data->mutex);
	}

	MUTEX_LOCK(shared_data->mutex);
	myqueue_push(&shared_data->queue, 0);
	MUTEX_UNLOCK(shared_data->mutex);

	return NULL;
}

void* consumer(void* arg) {
	struct shared_t* shared_data = (struct shared_t*)arg;
	int sum = 0;
	int done = 0;

	while (!done) {
		MUTEX_LOCK(shared_data->mutex);
		if (!myqueue_is_empty(&shared_data->queue)) {
			int val = myqueue_pop(&shared_data->queue);
			if (val == 0) {
				done = 1;
			} else {
				sum += val;
			}
		}
		MUTEX_UNLOCK(shared_data->mutex);
	}

	printf("Sum: %d\n", sum);
	return NULL;
}

int main() {
	pthread_t t1, t2;

	struct shared_t shared_data;

	myqueue_init(&shared_data.queue);
	MUTEX_INIT(shared_data.mutex);

	pthread_create(&t1, NULL, producer, &shared_data);
	pthread_create(&t2, NULL, consumer, &shared_data);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	return EXIT_SUCCESS;
}
