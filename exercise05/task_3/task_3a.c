#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#define SHM_NAME "/csbb9638_shared_mem"
#define SEM_EMPTY_NAME "/csbb9638_sem_empty"
#define SEM_FULL_NAME "/csbb9638_sem_full"

typedef struct shared_data {
	unsigned long long result;
	unsigned long long buffer[];
} shared_data;

void producer(int N, int K, int L, shared_data* data, sem_t* sem_empty, sem_t* sem_full) {
	for (int i = 0; i < K; i++) {
		sem_wait(sem_empty);
		data->buffer[i % L] = N * (i + 1) + 17;
		sem_post(sem_full);
	}
}
void consumer(int K, int L, shared_data* data, sem_t* sem_empty, sem_t* sem_full) {
	unsigned long long sum = 0;

	for (int i = 0; i < K; i++) {
		sem_wait(sem_full);
		sum += data->buffer[i % L];
		sem_post(sem_empty);
	}

	data->result = sum;
}
void validate_result(unsigned long long result, const unsigned long long K,
                     const unsigned long long N) {
	for (unsigned long long i = 0; i < K; i++) {
		result -= N * (i + 1) + 17;
	}
	printf("Checksum: %llu\n", result);
}

int main(int argc, const char* argv[]) {
	if (argc != 4) {
		printf("Usage: %s <N> <K> <L>\n", argv[0]);
		return EXIT_FAILURE;
	}

	const unsigned long long N = atoll(argv[1]);
	const unsigned long long K = atoll(argv[2]);
	const unsigned long long L = atoll(argv[3]);

	size_t mem_size = sizeof(shared_data) + L * sizeof(unsigned long long);

	int shm_file_desc = shm_open(SHM_NAME, O_CREAT | O_RDWR, S_IWUSR | S_IRUSR);
	if (shm_file_desc == -1) {
		perror("shm_open");
		return EXIT_FAILURE;
	}

	if (ftruncate(shm_file_desc, mem_size) == -1) {
		perror("ftruncate");
		return EXIT_FAILURE;
	}

	shared_data* data = mmap(0, mem_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_file_desc, 0);
	if (data == MAP_FAILED) {
		perror("mmap");
		return EXIT_FAILURE;
	}

	sem_t* sem_empty = sem_open(SEM_EMPTY_NAME, O_CREAT | O_EXCL, S_IRUSR | S_IRUSR, L);
	sem_t* sem_full = sem_open(SEM_FULL_NAME, O_CREAT | O_EXCL, S_IRUSR | S_IRUSR, 0);

	if (sem_empty == SEM_FAILED || sem_full == SEM_FAILED) {
		perror("sem_open");
		return EXIT_FAILURE;
	}

	pid_t pid1 = fork();
	if (pid1 == -1) {
		perror("fork failed");
		return EXIT_FAILURE;
	}

	if (pid1 == 0) {
		producer(N, K, L, data, sem_empty, sem_full);
		return EXIT_SUCCESS;
	}

	pid_t pid2 = fork();
	if (pid2 == -1) {
		perror("fork failed");
		return EXIT_FAILURE;
	}

	if (pid2 == 0) {
		consumer(K, L, data, sem_empty, sem_full);
		return EXIT_SUCCESS;
	}

	wait(NULL);
	wait(NULL);

	sem_close(sem_empty);
	sem_close(sem_full);
	sem_unlink(SEM_EMPTY_NAME);
	sem_unlink(SEM_FULL_NAME);

	printf("Result: %llu\n", data->result);
	validate_result(data->result, K, N);
	munmap(data, mem_size);
	close(shm_file_desc);
	shm_unlink(SHM_NAME);

	return EXIT_SUCCESS;
}
