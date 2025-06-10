CC = gcc
CFLAGS = -Wall -fPIC

all: some_library.so my_program

some_library.so: some_library.c
	$(CC) -shared $(CFLAGS) -o some_library.so some_library.c

my_program: my_program.c
#	$(CC) -static -o my_program my_program.c   # Commented for ZID-GPL
	$(CC) -o my_program my_program.c

clean:
	rm -f *.o *.s my_program some_library.so some_program
