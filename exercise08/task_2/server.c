#include "common.h"
#include <netinet/in.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

typedef struct client {
	int socket_fd;
	char username[UNAME_LEN];
	int is_admin;
	pthread_t client;
	pthread_t* listener;
	struct server* server_ref;
} client_t;

typedef char admins_t[ADMINS][UNAME_LEN];

typedef struct server {
	atomic_int clients_alive;
	pthread_mutex_t lock;
	pthread_cond_t all_clients_disconnected;
	pthread_mutex_t print_lock;
	pthread_t listener;
	struct sockaddr_in addr;
	int server_socket_fd;
	admins_t admins;
} server_t;

void* listener(void* args);
void* client(void* args);
void extractAdmins(int argc, char const* argv[], admins_t dest);
int is_admin(admins_t admins, char user[UNAME_LEN]);

int main(int argc, char const* argv[]) {
	if (argc < 3) {
		printf("Usage: %s <port> <...admins[5]>\n", argv[0]);
		return EXIT_FAILURE;
	}

	server_t server = {
        .addr = {
            .sin_family = AF_INET,
            .sin_addr = {
                .s_addr = htonl(INADDR_ANY),
            },
            .sin_port = htons(atoi(argv[1])),
        },
        .server_socket_fd = socket(AF_INET, SOCK_STREAM, 0),
        .clients_alive = 0
    };

	extractAdmins(argc, argv, server.admins);

	pthread_mutex_init(&server.lock, NULL);
	pthread_cond_init(&server.all_clients_disconnected, NULL);
	pthread_mutex_init(&server.print_lock, NULL);

	if (server.server_socket_fd < 0) {
		perror("socket");
		return EXIT_FAILURE;
	}

	if (bind(server.server_socket_fd, (struct sockaddr*)&server.addr, sizeof(server.addr)) < 0) {
		perror("bind");
		return EXIT_FAILURE;
	}

	if (pthread_create(&server.listener, NULL, listener, &server) != 0) {
		perror("pthread_create");
		return EXIT_FAILURE;
	}

	if (pthread_join(server.listener, NULL) != 0) {
		perror("pthread_join");
		return EXIT_FAILURE;
	}

	if (server.clients_alive > 0) {
		pthread_mutex_lock(&server.lock);
		printf("Server is shutting down. Waiting for %d client(s) to disconnect.\n",
		       server.clients_alive);
		while (server.clients_alive > 0) {
			pthread_cond_wait(&server.all_clients_disconnected, &server.lock);
		}
		pthread_mutex_unlock(&server.lock);
	} else {
		printf("Server is shutting down.\n");
	}

	pthread_mutex_destroy(&server.lock);
	pthread_cond_destroy(&server.all_clients_disconnected);
	pthread_mutex_destroy(&server.print_lock);
	close(server.server_socket_fd);
	return EXIT_SUCCESS;
}

void extractAdmins(int argc, char const* argv[], admins_t dest) {
	int num_admins = (argc - 2) < ADMINS ? (argc - 2) : ADMINS;
	for (int i = 0; i < num_admins; i++) {
		strncpy(dest[i], argv[i + 2], UNAME_LEN);
	}
}

void* listener(void* args) {
	server_t* server = (server_t*)args;

	if (listen(server->server_socket_fd, BACKLOG_QUEUE_LEN) != 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}

	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_mutex_lock(&server->print_lock);
	printf("Listening on port %d.\n", ntohs(server->addr.sin_port));
	pthread_mutex_unlock(&server->print_lock);

	while (1) {
		client_t* new_client = malloc(sizeof(client_t));
		if (!new_client) {
			perror("malloc");
			continue;
		}

		new_client->socket_fd = accept(server->server_socket_fd, NULL, NULL);
		if (new_client->socket_fd < 0) {
			perror("accept");
			free(new_client);
			continue;
		}

		int un_len = recv(new_client->socket_fd, new_client->username, UNAME_LEN, 0);
		new_client->username[un_len - 1] = '\0';
		new_client->is_admin = is_admin(server->admins, new_client->username);
		new_client->listener = &server->listener;
		new_client->server_ref = server;

		pthread_mutex_lock(&server->print_lock);
		printf("%s connected%s.\n", new_client->username, new_client->is_admin ? " (admin)" : "");
		pthread_mutex_unlock(&server->print_lock);

		server->clients_alive++;

		if (pthread_create(&new_client->client, NULL, client, new_client) != 0) {
			perror("pthread_create");
			close(new_client->socket_fd);
			free(new_client);
			continue;
		}

		pthread_detach(new_client->client);
	}
	return NULL;
}

void* client(void* args) {
	client_t* client = (client_t*)args;
	server_t* server = client->server_ref;
	char buffer[MSG_SIZE];
	int received;

	while ((received = recv(client->socket_fd, buffer, MSG_SIZE, 0)) > 0) {
		buffer[received - 1] = '\0';
		pthread_mutex_lock(&server->print_lock);
		printf("%s: %s\n", client->username, buffer);
		pthread_mutex_unlock(&server->print_lock);

		if (client->is_admin && strcmp(buffer, "/shutdown") == 0) {
			pthread_cancel(*client->listener);
			break;
		}
	}

	pthread_mutex_lock(&server->print_lock);
	printf("%s disconnected%s.\n", client->username, client->is_admin ? " (admin)" : "");
	pthread_mutex_unlock(&server->print_lock);

	close(client->socket_fd);
	free(client);

	server->clients_alive--;
	if (server->clients_alive == 0) {
		pthread_mutex_lock(&server->lock);
		pthread_cond_signal(&server->all_clients_disconnected);
		pthread_mutex_unlock(&server->lock);
	}
	return NULL;
}

int is_admin(admins_t admins, char user[UNAME_LEN]) {
	for (int i = 0; i < ADMINS; i++) {
		if (strncmp(admins[i], user, UNAME_LEN) == 0) {
			return 1;
		}
	}
	return 0;
}
