#include <err.h>
#include <errno.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

void signalHandler(int signal) {
	char* msg;
	switch (signal) {
		case SIGUSR1:
			msg = "Received SIGUSR1. Ignoring SIGUSR2\n";
			struct sigaction ignoreU2 = { .sa_handler = SIG_IGN };
			if (sigaction(SIGUSR2, &ignoreU2, NULL) == -1) {
				char* error = "Could not ignore SIGUSR2\n";
				write(STDERR_FILENO, error, strlen(error));
			}
			break;
		case SIGUSR2:
			msg = "Received SIGUSR2. Ignoring SIGUSR1\n";
			struct sigaction ignoreU1 = { .sa_handler = SIG_IGN };
			if (sigaction(SIGUSR1, &ignoreU1, NULL) == -1) {
				char* error = "Could not ignore SIGUSR1\n";
				write(STDERR_FILENO, error, strlen(error));
			}
			break;
		case SIGINT:
			msg = "Received SIGINT. Accept SIGUSR1 and SIGUSR2 again.\n";
			struct sigaction backToDefault = { .sa_handler = signalHandler };
			if (sigaction(SIGUSR1, &backToDefault, NULL) == -1) {
				char* error = "Could not accept SIGUSR1 signals again\n";
				write(STDERR_FILENO, error, strlen(error));
			}
			if (sigaction(SIGUSR2, &backToDefault, NULL) == -1) {
				char* error = "Could not accept SIGUSR2 signals again\n";
				write(STDERR_FILENO, error, strlen(error));
			}
			break;
		case SIGCONT: msg = "Received SIGCONT\n"; break;
		default: msg = "Received unhandled signal";
	}
	write(STDOUT_FILENO, msg, strlen(msg));
}

int main(void) {
	int signals[6] = { SIGINT, SIGSTOP, SIGCONT, SIGKILL, SIGUSR1, SIGUSR2 };

	struct sigaction sa;
	sa.sa_handler = signalHandler;

	for (int i = 0; i < 6; i++) {
		int type = signals[i];
		if (type == SIGSTOP || type == SIGKILL) {
			perror("Note: Cannot overwrite signal handler for SIGSTOP or SIGKILL");
			continue;
		}
		if (sigaction(type, &sa, NULL) == -1) {
			perror("sigaction()");
			return EXIT_FAILURE;
		}
	}

	const time_t work_seconds = 1;

	struct timespec work_duration = {
		.tv_sec = work_seconds,
	};

	struct timespec remaining = { 0 };

	while (true) {
		// simulate real workload
		if (nanosleep(&work_duration, &remaining) == -1 && errno == EINTR) {
			work_duration = remaining;
			continue;
		}

		// restore work_duration
		work_duration.tv_sec = work_seconds;
	}

	return EXIT_SUCCESS;
}