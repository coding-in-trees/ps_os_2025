CFLAGS = -std=c11 -Wall -Wextra -lrt -D_DEFAULT_SOURCE

.PHONY: all
all: scheduler service

scheduler: scheduler.c

service: service.c

.PHONY: clean
clean:
	$(RM) -f service scheduler