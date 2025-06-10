CC = gcc
CFLAGS = -Wall -fPIC
LDFLAGS = -shared

PLUGINS = caesar_cipher.so xor_string.so reverse_string.so rotate_string.so library_one.so library_two.so

all: map_string $(PLUGINS)

map_string: map_string.c
	$(CC) -o $@ $< -ldl

caesar_cipher.so: caesar_cipher.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

xor_string.so: xor_string.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

reverse_string.so: reverse_string.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

rotate_string.so: rotate_string.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

library_one.so: library_one.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

library_two.so: library_two.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

clean:
	rm -f map_string *.so
