#include "thread_pool.h"

void *worker(void*arg) {
    thread_pool *pool = (thread_pool*)arg;

    while (true)
    {
        pthread_mutex_lock(pool->lockQueue);

        if (pool->shutdown)
        {
            pthread_mutex_unlock(pool->lockQueue);
            break;
        }

        while (job_queue_is_empty(pool->jobs) && !pool->shutdown)
        {
            pthread_cond_wait(pool->addedJobs, pool->lockQueue);
        }

        if (pool->shutdown && job_queue_is_empty(pool->jobs))
        {
            pthread_mutex_unlock(pool->lockQueue);
            break;
        }

        worker_job job = job_queue_pop(pool->jobs);
        pthread_mutex_unlock(pool->lockQueue);

        job.start_routine(job.arg);
        sem_post(job.id);
    }
    return NULL;
}

void pool_create(thread_pool* pool, size_t size) {
	pool->shutdown = false;
    pool->size = size;
    pool->addedJobs = malloc(sizeof(pthread_cond_t));
    if (pool->addedJobs == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    pool->lockQueue = malloc(sizeof(pthread_mutex_t));
    if (pool->lockQueue == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    pool->jobs = malloc(sizeof(job_queue));
    
    pthread_mutex_init(pool->lockQueue, NULL);
	job_queue_init(pool->jobs);
    pthread_cond_init(pool->addedJobs, NULL);

    pool->workers = malloc(sizeof(pthread_t) * size);
    if (pool->workers == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    size_t threadId = 0;
    for (; threadId < size; threadId++)
    {
        if (pthread_create(&pool->workers[threadId], NULL, worker, pool) != 0)
        {
            perror("pthread_create");
            pool->shutdown = true;
            break;
        }
    }
    
    if (pool->shutdown)
    {
        for (size_t i = 0; i < threadId; i++)
        {
            if (pthread_join(pool->workers[i], NULL) != 0) {
                perror("pthread_join");
                exit(EXIT_FAILURE);
            }
        }
        free(pool->workers);
        free(pool->lockQueue);
        free(pool->addedJobs);
        free(pool->jobs);
        exit(EXIT_FAILURE);
    } 
}

job_id pool_submit(thread_pool* pool, job_function start_routine, job_arg arg) {
    worker_job job = {.id = malloc(sizeof(sem_t)), .arg = arg, .start_routine = start_routine};
    if (job.id == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    
    sem_init(job.id, 0, 0);
    
    pthread_mutex_lock(pool->lockQueue);
    job_queue_push(pool->jobs, job);
    pthread_cond_signal(pool->addedJobs);
    pthread_mutex_unlock(pool->lockQueue);
    return job.id;
}

void pool_await(job_id id) {
    sem_wait(id);
    sem_destroy(id);
    free(id);
}

void pool_destroy(thread_pool* pool) {
    pthread_mutex_lock(pool->lockQueue);
    pthread_cond_broadcast(pool->addedJobs);
    pool->shutdown = true;
    pthread_mutex_unlock(pool->lockQueue);

    for (size_t i = 0; i < pool->size; i++)
    {
        if (pthread_join(pool->workers[i], NULL) != 0) {
            perror("pthread_join");
            exit(EXIT_FAILURE);
        }
    }

    pthread_mutex_destroy(pool->lockQueue);
    pthread_cond_destroy(pool->addedJobs);

    while (!job_queue_is_empty(pool->jobs)) {
        worker_job job = job_queue_pop(pool->jobs);
        sem_destroy(job.id);
        free(job.id);
    }

    free(pool->workers);
    free(pool->lockQueue);
    free(pool->addedJobs);
    free(pool->jobs);
}

void job_queue_init(job_queue* q) {
	STAILQ_INIT(q);
}

bool job_queue_is_empty(job_queue* q) {
	return STAILQ_EMPTY(q);
}
void job_queue_push(job_queue* q, worker_job job) {
	struct job_queue_entry* entry = malloc(sizeof(struct job_queue_entry));
	entry->value = job;
	STAILQ_INSERT_TAIL(q, entry, entries);
}

worker_job job_queue_pop(job_queue* q) {
	assert(!job_queue_is_empty(q));
	struct job_queue_entry* entry = STAILQ_FIRST(q);
	const worker_job value = entry->value;
	STAILQ_REMOVE_HEAD(q, entries);
	free(entry);
	return value;
}