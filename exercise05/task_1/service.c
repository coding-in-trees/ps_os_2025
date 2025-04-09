#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define MESSAGE_SIZE 4096

typedef struct {
	char content[MESSAGE_SIZE];
} Message;

int main(int argc, char const* argv[]) {
	if (argc != 3) {
		printf("Usage: %s <message_queue_name> <priority>\n", argv[0]);
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

	if (atoi(argv[2]) > 99 || atoi(argv[2]) < 2) {
		printf("Priority on this linux machine needs to be inbetween 2 and 99.\n");
		return EXIT_FAILURE;
	}

	mqd_t schedule = mq_open(argv[1], O_WRONLY);
	if (schedule == -1) {
		perror("mq_open");
		return EXIT_FAILURE;
	}

	Message message;
	if (fgets(message.content, sizeof(message.content), stdin) == NULL) {
		perror("fgets");
		return EXIT_FAILURE;
	}

	if (mq_send(schedule, (char*)&message, sizeof(Message), atoi(argv[2])) == -1) {
		perror("mq_send");
		return EXIT_FAILURE;
	}

	if (mq_close(schedule) == -1) {
		perror("mq_close");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
