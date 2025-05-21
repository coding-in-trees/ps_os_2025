#include "my_pthread_barrier.h"
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct player_s player_t;

typedef struct {
	uint32_t n;
	player_t* players;
	my_pthread_barrier_t barrier;
	int game_done;
} game_state;

typedef struct player_s {
	pthread_t thread;
	uint32_t id;
	int active;
	int rolled;
	game_state* game;
} player_t;

void* player_thread(void* arg) {
	player_t* player = (player_t*)arg;

	while (!player->game->game_done) {
		if (player->active) {
			int r1 = rand() % 6 + 1;
			int r2 = rand() % 6 + 1;
			player->rolled = r1 + r2;
			printf("Player %d rolled a %d and a %d, summing up to %d\n", player->id, r1, r2,
			       player->rolled);
		}

		int player_is_game_master = my_pthread_barrier_wait(&player->game->barrier);
		if (player_is_game_master == -1) {
			int max = 2;
			int alive = 0;
			int last = -1;
			int repeat_round = 1;

			for (uint32_t i = 0; i < player->game->n; ++i) {
				player_t* p = &player->game->players[i];
				if (!p->active) continue;
				alive++;
				if (last != -1 && p->rolled != last) repeat_round = 0;
				last = p->rolled;
				if (p->rolled > max) max = p->rolled;
			}

			if (repeat_round && alive > 1) {
				printf("Repeating round\n---------------------\n");
			} else if (alive > 1) {
				for (uint32_t i = 0; i < player->game->n; ++i) {
					player_t* p = &player->game->players[i];
					if (p->active && p->rolled == max) {
						p->active = 0;
						printf("Eliminating player %d\n", p->id);
					}
				}
				printf("---------------------\n");
			}

			if (alive == 1) {
				for (uint32_t i = 0; i < player->game->n; ++i) {
					player_t* p = &player->game->players[i];
					if (p->active) {
						printf("Player %d has won the game\n", p->id);
					}
				}
				player->game->game_done = 1;
			}
		}

		my_pthread_barrier_wait(&player->game->barrier);
	}

	return NULL;
}

int main(int argc, char const* argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <num_players>\n", argv[0]);
		return EXIT_FAILURE;
	}

	uint32_t n = atoi(argv[1]);
	if (n < 2) {
		fprintf(stderr, "Number of players must be >= 2\n");
		return EXIT_FAILURE;
	}

	srand(time(NULL));

	game_state state = {
		.n = n,
		.players = calloc(n, sizeof(player_t)),
		.game_done = 0,
	};

	my_pthread_barrier_init(&state.barrier, NULL, n);

	for (uint32_t i = 0; i < n; ++i) {
		state.players[i].active = 1;
		state.players[i].id = i;
		state.players[i].rolled = 0;
		state.players[i].game = &state;

		if (pthread_create(&state.players[i].thread, NULL, player_thread, &state.players[i]) != 0) {
			perror("pthread_create");
			state.n = i; // only join what was created
			break;
		}
	}

	for (uint32_t i = 0; i < state.n; ++i) {
		pthread_join(state.players[i].thread, NULL);
	}

	my_pthread_barrier_destroy(&state.barrier);
	free(state.players);
	return EXIT_SUCCESS;
}
