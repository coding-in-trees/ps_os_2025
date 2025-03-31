#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        return EXIT_FAILURE;
    }
    
    int pipefd[2];
    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return EXIT_FAILURE;
    }

    pid_t currProcess = fork();
    if (currProcess < 0)
    {
        perror("pipe");
        return EXIT_FAILURE;
    }
    if (currProcess == 0)
    {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        // replace forked process
        execlp("cat", "cat", argv[1], argv[2], NULL);
        // exit if the execlp failed
        perror("execlp");
        return EXIT_FAILURE;
    }

    close(pipefd[1]);
    dup2(pipefd[0], STDIN_FILENO);
    close(pipefd[0]);

    // hand over main process to cut
    execlp("cut", "cut", "-c", "22-", NULL);
    perror("execlp");
    return EXIT_FAILURE;
}
