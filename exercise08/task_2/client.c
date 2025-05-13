#include "common.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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

	char* username = argv[2];

	struct sockaddr_in server_addr;

	int client_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket_fd < 0) {
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(atoi(argv[1]));
	if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) < 0) {
		perror("inet_pton");
		close(client_socket_fd);
		exit(EXIT_FAILURE);
	}

	if (connect(client_socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
		perror("connect");
		close(client_socket_fd);
		exit(EXIT_FAILURE);
	}

	if (send(client_socket_fd, username, user_len + 1, 0) < 0) {
		perror("send");
		close(client_socket_fd);
		return EXIT_FAILURE;
	}

	while (1) {
		char message_buf[MSG_SIZE];
		fgets(message_buf, MSG_SIZE, stdin);
		message_buf[strlen(message_buf) - 1] = '\0';

		if (strcmp(message_buf, "/quit") == 0) {
			break;
		}

		send(client_socket_fd, message_buf, MSG_SIZE, 0);

		if (strcmp(message_buf, "/shutdown") == 0) {
			break;
		}
	}

	close(client_socket_fd);
	return EXIT_SUCCESS;
}
