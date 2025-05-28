#include <assert.h>
#include <pthread.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#include "allocator.h"
#include "allocator_tests.h"
#include "membench.h"

#define ALIGNMENT 8
#define HEADER_SIZE (sizeof(block_header_t))

typedef struct block_header {
    size_t size;
    struct block_header* next;
} block_header_t;

static void* memory_pool = NULL;
static size_t pool_size = 0;
static block_header_t* free_list_head = NULL;
static pthread_mutex_t alloc_mutex = PTHREAD_MUTEX_INITIALIZER;

// Align size to next multiple of ALIGNMENT
static size_t align(size_t size) {
    return (size + ALIGNMENT - 1) & ~(ALIGNMENT - 1);
}

void my_allocator_init(size_t total_size) {
    pthread_mutex_lock(&alloc_mutex);

    if (memory_pool != NULL) {
        pthread_mutex_unlock(&alloc_mutex);
        return;
    }

    total_size = align(total_size);
    memory_pool = mmap(NULL, total_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (memory_pool == MAP_FAILED) {
        memory_pool = NULL;
        pthread_mutex_unlock(&alloc_mutex);
        return;
    }

    pool_size = total_size;
    free_list_head = (block_header_t*)memory_pool;
    free_list_head->size = total_size - HEADER_SIZE;
    free_list_head->next = NULL;

    pthread_mutex_unlock(&alloc_mutex);
}

void my_allocator_destroy(void) {
    pthread_mutex_lock(&alloc_mutex);

    if (memory_pool != NULL) {
        munmap(memory_pool, pool_size);
        memory_pool = NULL;
        free_list_head = NULL;
        pool_size = 0;
    }

    pthread_mutex_unlock(&alloc_mutex);
}

void* my_malloc(size_t size) {
    if (size == 0) return NULL;

    pthread_mutex_lock(&alloc_mutex);

    size = align(size);
    block_header_t* best_prev = NULL;
    block_header_t* best_block = NULL;
    block_header_t* prev = NULL;
    block_header_t* curr = free_list_head;

    // Best-fit search
    while (curr != NULL) {
        if (curr->size >= size) {
            if (best_block == NULL || curr->size < best_block->size) {
                best_block = curr;
                best_prev = prev;
            }
        }
        prev = curr;
        curr = curr->next;
    }

    if (best_block == NULL) {
        pthread_mutex_unlock(&alloc_mutex);
        return NULL;
    }

    size_t remaining = best_block->size - size;

    if (remaining >= HEADER_SIZE + ALIGNMENT) {
        block_header_t* new_block = (block_header_t*)((char*)best_block + HEADER_SIZE + size);
        new_block->size = remaining - HEADER_SIZE;
        new_block->next = best_block->next;

        best_block->size = size;

        if (best_prev == NULL) {
            free_list_head = new_block;
        } else {
            best_prev->next = new_block;
        }
    } else {
        if (best_prev == NULL) {
            free_list_head = best_block->next;
        } else {
            best_prev->next = best_block->next;
        }
    }

    pthread_mutex_unlock(&alloc_mutex);
    return (char*)best_block + HEADER_SIZE;
}

void my_free(void* ptr) {
    if (!ptr) return;

    pthread_mutex_lock(&alloc_mutex);

    block_header_t* block = (block_header_t*)((char*)ptr - HEADER_SIZE);
    block_header_t* curr = free_list_head;
    block_header_t* prev = NULL;

    // Insert in address order
    while (curr != NULL && curr < block) {
        prev = curr;
        curr = curr->next;
    }

    // Try to coalesce with previous block
    if (prev && (char*)prev + HEADER_SIZE + prev->size == (char*)block) {
        prev->size += HEADER_SIZE + block->size;
        block = prev;
    } else {
        block->next = curr;
        if (prev) {
            prev->next = block;
        } else {
            free_list_head = block;
        }
    }

    // Try to coalesce with next block
    if (curr && (char*)block + HEADER_SIZE + block->size == (char*)curr) {
        block->size += HEADER_SIZE + curr->size;
        block->next = curr->next;
    }

    pthread_mutex_unlock(&alloc_mutex);
}

// ------

void run_bench(void) {
#if THREAD_LOCAL_ALLOCATOR
	run_membench_thread_local(my_allocator_init, my_allocator_destroy, my_malloc, my_free);
#else
	run_membench_global(my_allocator_init, my_allocator_destroy, my_malloc, my_free);
#endif
}

void run_tests(void) {
	test_best_fit_allocator();
}
