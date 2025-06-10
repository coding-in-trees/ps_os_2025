CC = gcc
CFLAGS = -Wall -Wextra -fPIC -O2
LDFLAGS = -shared -ldl
TARGET = malloc_spy.so
SRC = malloc_spy.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TARGET)
