CFLAGS = -std=c11 -Wall -Wextra

.PHONY: all
all: main

.PHONY: create_key
create_key:
	@echo "Creating key.c"
	@echo -e """#include \"key.h\"\n\nconst char* get_key() {\n\treturn \"$(shell whoami)\"; // here you need to replace \"your_username\" with the username of your account. Look at whoami(1).\n}""" > key.c

.PHONY: clean
clean:
	rm -f key.c main *.o

main: main.o key.o
	@echo "Linking main"
	$(CC) main.o key.o -o main

main.o: main.c
	@echo "Compiling main.c"
	$(CC) $(CFLAGS) -c main.c

key.o: create_key key.c
	@echo "Compiling key.c"
	$(CC) $(CFLAGS) -c key.c
