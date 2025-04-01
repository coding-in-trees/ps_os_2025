#include <fcntl.h>
#include <limits.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define FIFO_DIR "/tmp/csbb9638_"
#define MAX_CLIENTS 10
#define BUFFER_SIZE PIPE_BUF

void cleanup_fifos(char* clients[], int num_clients) {
	for (int i = 0; i < num_clients; i++) {
		char fifo_path[256];
		snprintf(fifo_path, sizeof(fifo_path), "%s%s_fifo", FIFO_DIR, clients[i]);
		unlink(fifo_path);
	}
}

int setup_fifos(int num_clients, char* clients[], char* argv[]) {
	for (int i = 0; i < num_clients; i++) {
		clients[i] = argv[i + 1];
		char fifo_path[256];
		snprintf(fifo_path, sizeof(fifo_path), "%s%s_fifo", FIFO_DIR, clients[i]);

		if (mkfifo(fifo_path, 0666) == -1) {
			perror("mkfifo");
			cleanup_fifos(clients, i);
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}

int open_client_channels(struct pollfd fds[], int num_clients, char* clients[]) {
	for (int i = 0; i < num_clients; i++) {
		char fifo_path[256];
		snprintf(fifo_path, sizeof(fifo_path), "%s%s_fifo", FIFO_DIR, clients[i]);
		fds[i].fd = open(fifo_path, O_RDONLY);
		fds[i].events = POLLIN;

		if (fds[i].fd == -1) {
			perror("open");
			cleanup_fifos(clients, num_clients);
			return EXIT_FAILURE;
		}
		printf("%s connected.\n", clients[i]);
	}
	return EXIT_SUCCESS;
}

int newMessage(struct pollfd fds[], char* clients[], int clientRef, int* message_count) {
	char buffer[BUFFER_SIZE];
	ssize_t bytes_read = read(fds[clientRef].fd, buffer, BUFFER_SIZE - 1);

	if (bytes_read == 0) {
		return 0;
	}

	buffer[bytes_read] = '\0';
	(*message_count)++;
	printf("Message %d: \"%s\" from %s\n", *message_count, buffer, clients[clientRef]);
	return 1;
}

void clientHangup(struct pollfd fds[], char* clients[], int* num_clients, int* clientRef) {
	printf("%s disconnected.\n", clients[*clientRef]);
	close(fds[*clientRef].fd);
	for (int j = *clientRef; j < *num_clients - 1; j++) {
		fds[j] = fds[j + 1];
		clients[j] = clients[j + 1];
	}
	(*num_clients)--;
	(*clientRef)--;
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		fprintf(stderr, "Usage: %s <client1> <client2> ...\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	int num_clients = argc - 1;
	char* clients[MAX_CLIENTS];
	struct pollfd fds[MAX_CLIENTS];
	int message_count = 0;

	if (setup_fifos(num_clients, clients, argv) == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}

	if (open_client_channels(fds, num_clients, clients) == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}

	while (num_clients > 0) {
		if (poll(fds, num_clients, -1) == -1) {
			cleanup_fifos(clients, num_clients);
			return EXIT_FAILURE;
		}
		for (int cRef = 0; cRef < num_clients; cRef++) {
			if (!(fds[cRef].revents & POLLIN)) {
				continue;
			}
			if (newMessage(fds, clients, cRef, &message_count)) {
				continue;
			}
			clientHangup(fds, clients, &num_clients, &cRef);
		}
	}

	printf("All clients disconnected.\n");
	cleanup_fifos(clients, argc - 1);
	return EXIT_SUCCESS;
}
