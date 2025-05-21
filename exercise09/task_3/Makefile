CFLAGS = -std=c11 -Wall -Wextra -pedantic
LDFLAGS = -pthread

TARGETS = task_3

.PHONY: all clean

all: $(TARGETS)

clean:
	$(RM) $(TARGETS) *.o

task_3: task_3.o my_pthread_barrier.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

task_3.o: task_3.c my_pthread_barrier.h
	$(CC) $(CFLAGS) -c task_3.c

my_pthread_barrier.o: my_pthread_barrier.c my_pthread_barrier.h
	$(CC) $(CFLAGS) -c my_pthread_barrier.c
