#include <assert.h>
#include <pthread.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#include "allocator.h"
#include "allocator_tests.h"
#include "membench.h"

#define BLOCK_SIZE 1024

static _Thread_local void* memory_pool = NULL;
static _Thread_local void* free_list_head = NULL;
static _Thread_local size_t pool_size = 0;

static void* get_next(void* block) {
    return *(void**)block;
}

static void set_next(void* block, void* next) {
    *(void**)block = next;
}

void my_allocator_init(size_t size) {
    if (memory_pool != NULL) {
        return;
    }

    memory_pool = mmap(NULL, size, PROT_READ | PROT_WRITE,
                       MAP_PRIVATE | MAP_ANON, -1, 0);

    if (memory_pool == MAP_FAILED) {
        memory_pool = NULL;
        return;
    }

    pool_size = size;

    size_t num_blocks = size / BLOCK_SIZE;

    free_list_head = memory_pool;
    char* current = (char*)memory_pool;

    for (size_t i = 0; i < num_blocks - 1; ++i) {
         char* next = current + BLOCK_SIZE;
        set_next(current, next);
        current = next;
    }
    set_next(current, NULL); // last block
}

void my_allocator_destroy(void) {
    if (memory_pool != NULL) {
        munmap(memory_pool, pool_size);
        memory_pool = NULL;
        free_list_head = NULL;
        pool_size = 0;
    }
}

void* my_malloc(size_t size) {
    if (size > BLOCK_SIZE - sizeof(void*)) {
        return NULL;
    }
    if (free_list_head == NULL) {
        return NULL;
    }

    void* block = free_list_head;
    free_list_head = get_next(block);

    return (char*)block + sizeof(void*);
}

void my_free(void* ptr) {
    if (!ptr) return;

    void* block = (char*)ptr - sizeof(void*);

    // Add to front of free list
    set_next(block, free_list_head);
    free_list_head = block;
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
	test_free_list_allocator();
}
