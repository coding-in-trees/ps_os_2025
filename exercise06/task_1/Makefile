CFLAGS = -std=c11 -Wall -Wextra -pthread

.PHONY: all
all: task_1_mutex task_1_atomic

task_1_mutex: task_1_mutex.c

task_1_atomic: task_1_atomic.c

.PHONY: clean
clean:
	$(RM) -f task_1_mutex task_1_atomic