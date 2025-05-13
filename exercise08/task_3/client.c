#include "common.h"
#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct client_t {
	char* username;
	struct sockaddr_in server_addr;
	pthread_t listener;
	int socket_fd;
};

void* listener(void* arg) {
	struct client_t* client = (struct client_t*)arg;

	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

	char buffer[MSG_SIZE];
	int received;
	while ((received = recv(client->socket_fd, buffer, MSG_SIZE, 0)) > 0) {
		buffer[received - 1] = '\0';
		printf("%s\n", buffer);
	}
	return NULL;
}

int main(int argc, char* argv[]) {
	if (argc != 3) {
		printf("Usage: %s <port> <username>", argv[0]);
		return EXIT_FAILURE;
	}

	size_t user_len = strlen(argv[2]);
	if (user_len >= UNAME_LEN) {
		printf("Your username can be at most %d characters long!", UNAME_LEN);
		return EXIT_FAILURE;
	}

	struct client_t client = {
		.username = argv[2],
		.socket_fd = socket(AF_INET, SOCK_STREAM, 0),
		.server_addr.sin_family = AF_INET,
		.server_addr.sin_port = htons(atoi(argv[1])),
	};

	if (client.socket_fd < 0) {
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}

	if (inet_pton(AF_INET, "127.0.0.1", &client.server_addr.sin_addr) < 0) {
		perror("inet_pton");
		close(client.socket_fd);
		exit(EXIT_FAILURE);
	}

	if (connect(client.socket_fd, (struct sockaddr*)&client.server_addr, sizeof(client.server_addr)) <
	    0) {
		perror("connect");
		close(client.socket_fd);
		exit(EXIT_FAILURE);
	}

	if (send(client.socket_fd, client.username, user_len + 1, 0) < 0) {
		perror("send");
		close(client.socket_fd);
		return EXIT_FAILURE;
	}

	if (pthread_create(&client.listener, NULL, listener, &client)) {
		perror("pthread_create");
		close(client.socket_fd);
		return EXIT_FAILURE;
	}

	while (1) {
		char message_buf[MSG_SIZE];
		fgets(message_buf, MSG_SIZE, stdin);
		message_buf[strlen(message_buf) - 1] = '\0';

		if (strcmp(message_buf, "/quit") == 0) {
			break;
		}

		send(client.socket_fd, message_buf, MSG_SIZE, 0);

		if (strcmp(message_buf, "/shutdown") == 0) {
			break;
		}
	}

	pthread_cancel(client.listener);
	pthread_join(client.listener, NULL);
	close(client.socket_fd);
	return EXIT_SUCCESS;
}
