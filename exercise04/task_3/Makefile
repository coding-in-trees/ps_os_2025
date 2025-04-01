CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -D_POSIX_C_SOURCE=200809L

.PHONY: all
all: client server

client: client.c

server: server.c

.PHONY: clean
clean:
	$(RM) -f client server