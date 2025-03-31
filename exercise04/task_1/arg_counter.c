#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum exit_codes {
	NO_ARGUMENTS = 13,
	TOO_MANY_ARGUMENTS = 7,
	INVALID_OFFSET = 99,
	// extra constraint: every arg needs to be at leas 3 chars long
	INVALID_ARGUMENT = 98
};

int exit_with_code(int code) {
	printf("Usage: ./arg_counter <arg1> [arg2 ... arg10]\n");
	switch (code) {
		case NO_ARGUMENTS: printf("Error: you supplied too few arguments. Supply at least 1\n"); break;
		case TOO_MANY_ARGUMENTS: printf("Error: you supplied too many arguments. At max 10\n"); break;
		case INVALID_OFFSET:
			printf("Error: You didn't set the OFFSET env variable or it's value was not only digits\n");
			break;
		case INVALID_ARGUMENT:
			printf("Error: every argument needs to be at least 4 characters long.\n");
			break;
		default: break;
	}
	return code;
}

int main(int argc, char const* argv[]) {
	// too few arguments
	if (argc == 1) {
		return exit_with_code(NO_ARGUMENTS);
	}

	// too many arguments
	if (argc > 11) {
		return exit_with_code(TOO_MANY_ARGUMENTS);
	}

	// get OFFSET env variable if set
	char* offset_env = getenv("OFFSET");
	if (offset_env == NULL || strlen(offset_env) == 0 || offset_env[0] == '=') {
		return exit_with_code(INVALID_OFFSET);
	}

	// check for to short args
	for (int i = 1; i < argc; i++) {
		if (strlen(argv[i]) < 4) {
			return exit_with_code(INVALID_ARGUMENT);
		}
	}

	printf("Result: %i\n", argc - 1 + atoi(offset_env));
	return EXIT_SUCCESS;
}
