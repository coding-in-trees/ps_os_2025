CFLAGS = -std=c11 -Wall -Wextra -pedantic -D_DEFAULT_SOURCE 
LDFLAGS = -pthread

TARGETS = server

.PHONY:
all: $(TARGETS)

.PHONY:
clean:
	$(RM) $(TARGETS) *.o 

server: server.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

