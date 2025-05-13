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
	struct server* server_ref;
} client_t;

typedef char admins_t[ADMINS][UNAME_LEN];

typedef struct server {
	atomic_int clients_alive;
	pthread_mutex_t lock_client_socket_fds;
	int client_socket_fds[MAX_CLIENTS];
	pthread_mutex_t lock;
	pthread_cond_t all_clients_disconnected;
	pthread_t listener;
	struct sockaddr_in addr;
	int server_socket_fd;
	admins_t admins;
} server_t;

void* listener(void* args);
void* client(void* args);
void extractAdmins(int argc, char const* argv[], admins_t dest);
int is_admin(admins_t admins, char user[UNAME_LEN]);
void broadcast(server_t* server, int exclude_fd, char* msg);

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
	close(server.server_socket_fd);
	return EXIT_SUCCESS;
}

void broadcast(server_t* server, int exclude_fd, char* msg) {
	for (int i = 0; i < server->clients_alive; i++) {
		if (exclude_fd >= 0 && server->client_socket_fds[i] == exclude_fd) {
			continue;
		}
		send(server->client_socket_fds[i], msg, strlen(msg)+1, 0);
	}
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
	printf("Listening on port %d.\n", ntohs(server->addr.sin_port));

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
		if (server->clients_alive >= MAX_CLIENTS) {
			free(new_client);
			continue;
		}

		int un_len = recv(new_client->socket_fd, new_client->username, UNAME_LEN - 1, 0);
		if (un_len <= 0) {
			close(new_client->socket_fd);
			free(new_client);
			continue;
		}
		new_client->username[un_len] = '\0';
		new_client->is_admin = is_admin(server->admins, new_client->username);
		new_client->server_ref = server;

		char connect_msg[MSG_SIZE];
		snprintf(connect_msg, sizeof(connect_msg), "%s connected%s.", new_client->username,
		         new_client->is_admin ? " (admin)" : "");
		broadcast(server, new_client->socket_fd, connect_msg);

		pthread_mutex_lock(&server->lock_client_socket_fds);
		server->client_socket_fds[server->clients_alive] = new_client->socket_fd;
		pthread_mutex_unlock(&server->lock_client_socket_fds);
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

		char msg[MSG_SIZE + UNAME_LEN + 1];
		snprintf(msg, sizeof(msg), "%s: %s", client->username, buffer);
		broadcast(server, client->socket_fd, msg);

		if (client->is_admin && strcmp(buffer, "/shutdown") == 0) {
			pthread_cancel(server->listener);
			break;
		}
	}

	char disconnect_msg[MSG_SIZE];

	snprintf(disconnect_msg, sizeof(disconnect_msg), "%s disconnected%s.", client->username,
	         client->is_admin ? " (admin)" : "");

	broadcast(server, client->socket_fd, disconnect_msg);

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
