#define _GNU_SOURCE
#include <dlfcn.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

typedef void *(*malloc_type)(size_t);

static malloc_type real_malloc = NULL;

char* append_number(char* dst, size_t number, size_t len) {
	if (len == 0) {
		return dst;
	}

	if (number > 9) {
		char* new_dst = append_number(dst, number / 10, len);

		if (*new_dst != '\0') {
			return new_dst;
		}

		len -= (new_dst - dst);
		dst = new_dst;
	}

	if (len-- > 0) {
		const char digit = '0' + number % 10;
		*dst = digit;
	}

	if (len > 0) {
		*(++dst) = '\0';
	}

	return dst;
}

void *malloc(size_t size) {
	if (!real_malloc) {
		real_malloc = (malloc_type)dlsym(RTLD_NEXT, "malloc");
		if (!real_malloc) {
			_exit(1);
		}
	}

	char buf[64];
	char *p = buf;

	const char *prefix = "allocating ";
	p = stpncpy(p, prefix, sizeof(buf) - (p - buf));

	size_t remaining = sizeof(buf) - (p - buf);
	p = append_number(p, size, remaining);

	remaining = sizeof(buf) - (p - buf);
	const char *suffix = " bytes\n";
	p = stpncpy(p, suffix, remaining);

	size_t len = p - buf;

	write(1, buf, len);

	return real_malloc(size);
}
