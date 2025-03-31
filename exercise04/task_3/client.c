#include "server.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <client_name>\n", argv[0]);
		return EXIT_FAILURE;
	}

	char* client_name = argv[1];
	char fifo_path[256];
	snprintf(fifo_path, sizeof(fifo_path), "%s%s_fifo", FIFO_DIR, client_name);

	int fd = open(fifo_path, O_WRONLY);
	if (fd == -1) {
		perror("open");
		return EXIT_FAILURE;
	}

	char buffer[BUFFER_SIZE];
	while (1) {
		printf("Message: ");
		fgets(buffer, BUFFER_SIZE, stdin);

		size_t len = strlen(buffer);
		if (len > 0 && buffer[len - 1] == '\n') {
			buffer[len - 1] = '\0';
		}

		if (strlen(buffer) == 0) {
			close(fd);
			return EXIT_SUCCESS;
		}

		write(fd, buffer, strlen(buffer));
	}
}
