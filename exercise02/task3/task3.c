#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define NS 1000000000

struct pState {
	int n;
	unsigned long long s;
	int t;
	struct timespec stime;
	struct timespec etime;
};

double DR_p(const int t, const int n, const unsigned long long s) {
	unsigned long long h = 0;
	for (unsigned long long i = s; i--;) {
		h += (rand() % n + rand() % n + 2 == t);
	}
	return (double)h / s;
}

void printUsage(const char* programName) {
	printf("usage: %s <n> <s>\n", programName);
}

double elapsedTime(struct pState state) {
	time_t sec = state.etime.tv_sec - state.stime.tv_sec;
	time_t nsDiff = state.etime.tv_nsec - state.stime.tv_nsec;
	time_t nsElapsed = (sec * NS) + nsDiff;
	return nsElapsed / (double)NS;
}

void printProcessResults(struct pState state, double p, pid_t pid, int process) {
	printf("Child %d PID = %d. ", process, pid);
	printf("DR_p(%d,%d,%llu) = %f. ", state.t, state.n, state.s, p);
	printf("Elapsed time = %fs\n", elapsedTime(state));
}

void rollTheDice(struct pState state, int pNo) {
	pid_t childPid = getpid();
	srand(childPid);
	double probability = DR_p(state.t, state.n, state.s);

	int gotEndTime = clock_gettime(CLOCK_REALTIME, &state.etime);
	if (gotEndTime < 0) {
		perror("Could not get end time");
		exit(EXIT_FAILURE);
	}
    printProcessResults(state, probability, childPid, pNo);

	exit(EXIT_SUCCESS);
}

int main(int argc, char const* argv[]) {
	if (argc != 3) {
		printUsage(*argv);
		return EXIT_FAILURE;
	}

	struct pState childState = {
		.n = atoi(argv[1]),
		.s = atoll(argv[2]),
	};

	int possibleTargets = 2 * childState.n - 2;

	int gotStartTime = clock_gettime(CLOCK_REALTIME, &childState.stime);
	if (gotStartTime < 0) {
		perror("Could not get start time");
		return EXIT_FAILURE;
	}

    // spawn processes for every target no
	for (int p = 0; p <= possibleTargets; p++) {
		childState.t = p + 2;
		pid_t child = fork();
		if (child == -1) {
			perror("Failed to create child process");
			return EXIT_FAILURE;
		}
		if (child != 0) {
			continue;
		}
		rollTheDice(childState, p);
	}

    // wait for every process to complete
	for (int p = 0; p <= possibleTargets; p++) {
		pid_t exited = wait(NULL);
		if (exited < 0) {
			perror("Process didnt exit friendly");
			return EXIT_FAILURE;
		}
	}
	printf("Done.\n");
	return EXIT_SUCCESS;
}
