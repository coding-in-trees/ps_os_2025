CFLAGS = -std=c11 -Wall -Wextra -Wpedantic

.PHONY: all
all: client server

client: client.c

server: server.c

.PHONY: clean
clean:
	$(RM) -f client server