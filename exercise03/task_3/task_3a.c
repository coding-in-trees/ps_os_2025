#include <inttypes.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int64_t accuumulation = 0;

void* accumulateThread(void* n) {
	for (int64_t i = 1; i <= *(int64_t*)n; i++) {
		accuumulation += i;
	}
	return NULL;
}

int main(int argc, char const* argv[]) {
	if (argc != 2) {
		printf("Usage ./%s <N>\n", argv[0]);
	}
	int64_t n = atoll(argv[1]);
	printf("Accumulation: %lli\n", accuumulation);

	int child = fork();
	if (child == -1) {
		perror("Failed to create child process");
		return EXIT_FAILURE;
	} else if (child == 0) {
		// subProcess
		for (int64_t i = 1; i <= n; i++) {
			accuumulation += i;
		}
		exit(EXIT_SUCCESS);
	}
	// finish sub process
	wait(NULL);
	printf("Accumulation: %lli\n", accuumulation);

	// main
	pthread_t t;
	int thread = pthread_create(&t, NULL, accumulateThread, &n);
	if (thread != 0) {
		perror("Failed to create thread");
		return EXIT_FAILURE;
	}
	pthread_join(t, NULL);
	printf("Accumulation: %lli\n", accuumulation);
	return EXIT_SUCCESS;
}
