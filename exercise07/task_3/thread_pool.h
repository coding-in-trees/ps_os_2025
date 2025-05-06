#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <stddef.h> // for size_t
#include <semaphore.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/queue.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

typedef void* (*job_function)(void*);
typedef void* job_arg;
typedef sem_t *job_id;

/***
 * This is the stub of a simple job queue.
 */

typedef struct job {
	job_function start_routine;
	job_arg arg;
	job_id id;
} worker_job;

struct job_queue_entry {
	worker_job value; 
	STAILQ_ENTRY(job_queue_entry) entries;
};

STAILQ_HEAD(job_queue_head, job_queue_entry);
/***
 * This is the stub for the thread pool that uses the queue.
 * Implement at LEAST the Prototype functions below.
 */
typedef struct job_queue_head job_queue;

typedef struct {
	bool shutdown;
	size_t size;
	pthread_mutex_t *lockQueue;
	pthread_cond_t *addedJobs;
	pthread_t *workers;
	job_queue *jobs;
} thread_pool;

// Prototypes for REQUIRED functions
void pool_create(thread_pool* pool, size_t size);
job_id pool_submit(thread_pool* pool, job_function start_routine,
                   job_arg arg); // You need to define a datatype for the job_id (chose wisely).
void pool_await(job_id id);
void pool_destroy(thread_pool* pool);

void job_queue_init(job_queue *q);
bool job_queue_is_empty(job_queue *q);
void job_queue_push(job_queue *q, worker_job job);
worker_job job_queue_pop(job_queue *q);

#endif
