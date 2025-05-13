CFLAGS = -std=c11 -Wall -Wextra -pedantic -D_DEFAULT_SOURCE 
LDFLAGS = -pthread

TARGETS = server client

.PHONY:
all: $(TARGETS)

.PHONY:
clean:
	$(RM) $(TARGETS) *.o 

server: server.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

client: client.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<
