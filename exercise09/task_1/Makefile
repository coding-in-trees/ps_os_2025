CFLAGS = -std=c11 -Wall -Wextra -pedantic -D_DEFAULT_SOURCE -DUSE_MY_MUTEX=0
LDFLAGS = -pthread

TARGETS = task_1

.PHONY:
all: $(TARGETS)

.PHONY:
clean:
	$(RM) $(TARGETS) *.o 

task_1: task_1.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

