#include "my_pthread_barrier.h"

int my_pthread_barrier_init(my_pthread_barrier_t* barrier, UNUSED_PARAM void* attr, int count) {
	barrier->participants = count;
	barrier->waitCount = 0;
	if (pthread_cond_init(&barrier->release, NULL) < 0) {
		return -1;
	}
	return pthread_mutex_init(&barrier->lockCount, NULL);
}

int my_pthread_barrier_wait(my_pthread_barrier_t* barrier) {
	pthread_mutex_lock(&barrier->lockCount);
	barrier->waitCount++;
	if (barrier->waitCount < barrier->participants) {
		pthread_cond_wait(&barrier->release, &barrier->lockCount);
		pthread_mutex_unlock(&barrier->lockCount);
		return 0;
	}

	barrier->waitCount = 0;
	pthread_cond_broadcast(&barrier->release);
	pthread_mutex_unlock(&barrier->lockCount);
	return -1;
}

int my_pthread_barrier_destroy(my_pthread_barrier_t* barrier) {
	int destroy_cond = pthread_cond_destroy(&barrier->release);
	int destroy_mutex = pthread_mutex_destroy(&barrier->lockCount);
	return destroy_cond < 0 || destroy_mutex < 0 ? -1 : 0;
}