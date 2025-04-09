#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <mqueue.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define MESSAGE_SIZE 4096
#define MAX_NUMS 100

typedef struct {
	char content[MESSAGE_SIZE];
} Message;

int getNumbersFromMessage(int numbers[], int length, Message msg) {
	int i = 0;
	char* token = strtok(msg.content, " ");
	while (token != NULL && i < length) {
		numbers[i++] = atoi(token);
		token = strtok(NULL, " ");
	}
	return i;
}

void runTask(int numbers[], int length) {
	int min = INT_MAX;
	int max = INT_MIN;
	float mean = 0;
	for (int i = 0; i < length; i++) {
		usleep(500000);
		int num = numbers[i];
		min = num < min ? num : min;
		max = num > max ? num : max;
		mean += num;
		printf("\rStatistics progress: %d%%", (100 * (i + 1) / length));
		fflush(stdout);
	}
	mean /= length;
	printf("\nMax: %i, Min: %i, Mean: %.2f\n\n", max, min, mean);
}

const char* task_queue_name;
mqd_t task_queue;

void exit_gracefully() {
	printf("\nSutting down.\n");
	if (mq_close(task_queue) == -1) {
		perror("mq_close");
	}
	if (mq_unlink(task_queue_name) == -1) {
		perror("mq_unlink");
	}
	exit(EXIT_SUCCESS);
}

int main(int argc, char const* argv[]) {
	if (argc != 2) {
		printf("Usage: %s <message_queue_name>\n", argv[0]);
		return EXIT_FAILURE;
	}

	for (size_t i = 0; i < strlen(argv[1]); i++) {
		if ((i == 0 && argv[1][i] != '/') || (i != 0 && argv[1][i] == '/')) {
			printf("Error: message_queue_name must start with '/'\n and cannot include any '/' except "
			       "the leading one. The provided argument was %s\n",
			       argv[1]);
			return EXIT_FAILURE;
		}
	}

	task_queue_name = argv[1];

	struct mq_attr options = {
		.mq_maxmsg = 10,
		.mq_msgsize = MESSAGE_SIZE,
		// ignored for mq_open
		.mq_flags = 0,
		.mq_curmsgs = 0,
	};

	struct sigaction sa;
	sa.sa_handler = exit_gracefully;

	if (sigaction(SIGINT, &sa, NULL) == -1) {
		perror("sigaction");
		exit(EXIT_FAILURE);
	}

	task_queue = mq_open(task_queue_name, O_RDONLY | O_CREAT, S_IWUSR | S_IRUSR, &options);
	if (task_queue == -1) {
		perror("mq_open");
		return EXIT_FAILURE;
	}

	while (1) {
		Message new_msg;
		unsigned int prio;
		if (mq_receive(task_queue, (char*)&new_msg, sizeof(Message), &prio) == -1) {
			perror("mq_receive");
			exit_gracefully();
			return EXIT_FAILURE;
		}
		printf("Scheduling task with priority %u:\n", prio);

		int numbers[MAX_NUMS];
		int count = getNumbersFromMessage(numbers, MAX_NUMS, new_msg);
		runTask(numbers, count);
	}

	return EXIT_SUCCESS;
}
