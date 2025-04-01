#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char const* argv[]) {
	if (argc != 3) {
		return EXIT_FAILURE;
	}

	int pipefd1[2], pipefd2[2];
	if (pipe(pipefd1) == -1 || pipe(pipefd2) == -1) {
		perror("pipe");
		return EXIT_FAILURE;
	}

	pid_t catProcess = fork();
	if (catProcess < 0) {
		perror("pipe");
		return EXIT_FAILURE;
	}
	if (catProcess == 0) {
		close(pipefd1[0]);
		dup2(pipefd1[1], STDOUT_FILENO);
		close(pipefd1[1]);
		// replace forked process
		execlp("cat", "cat", argv[1], argv[2], NULL);
		// exit if the execlp failed
		perror("execlp");
		return EXIT_FAILURE;
	}

	pid_t grepProcess = fork();
	if (grepProcess < 0) {
		perror("pipe");
		return EXIT_FAILURE;
	}
	if (grepProcess == 0) {
		// read from pipefd1
		close(pipefd1[1]);
		dup2(pipefd1[0], STDIN_FILENO);
		close(pipefd1[0]);

		// prepare pipefd2
		close(pipefd2[0]);
		dup2(pipefd2[1], STDOUT_FILENO);
		close(pipefd2[1]);

		execlp("grep", "grep", "^;", NULL);
		// exit in case of grep fails
		perror("execlp");
		return EXIT_FAILURE;
	}

	// close unused file descriptors
	close(pipefd1[0]);
	close(pipefd1[1]);
	close(pipefd2[1]);

	dup2(pipefd2[0], STDIN_FILENO);
	close(pipefd2[0]);

	// hand over main process to cut
	execlp("cut", "cut", "-c", "22-", NULL);
	perror("execlp");
	return EXIT_FAILURE;
}
