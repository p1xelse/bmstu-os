#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

#define N 2
#define TIME_SLEEP 2

int main()
{
    int child[N];
    int fd[N];
    char mes[N][64] = {"aaa\n", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n"};

    if (pipe(fd) == -1)
    {
        perror("Error pipe!");
        exit(1);
    }

    printf("Parent process start! PID: %d, GROUP: %d\n", getpid(), getpgrp());

    for (int i = 0; i < N; i++)
    {
        child[i] = fork();

        if(child[i] == -1)
        {
            perror("Can\'t fork()\n");
            exit(1);
        }
        else if (!child[i])
        {
            close(fd[0]);
            write(fd[1], mes[i], strlen(mes[i]));
            printf("Message %d sent to parent! %s", i + 1, mes[i]);

            return 0;
        }        
        else
        {
            printf("Parent process: PID=%d; GROUP: %d, Child: PID=%d\n", getpid(), getpgrp(), child[i]);
        }
    }

	for (int i = 0; i < N; i++)
	{
		int status;

		pid_t child_pid = wait(&status);

        if(child_pid == -1)
        {
            perror("Error wait\n");
            exit(1);
        }

		printf("Child has terminated: PID=%d. Status: %d\n", child_pid, status);

		if (WIFEXITED(status))
		{
			printf("Child process %d terminated. Code: %d\n", child_pid, WEXITSTATUS(status));
		}
		else if (WIFSIGNALED(status))
		{
			printf("Child process %d terminated from signal with code: %d\n", child_pid, WTERMSIG(status));
		}
		else if (WIFSTOPPED(status))
		{
			printf("Child process %d terminated stopped. Number signal: %d\n", child_pid, WSTOPSIG(status));
		}
	}

    printf("\nMessage receive:\n");
    close(fd[1]);
    read(fd[0], mes[0], 64);
    printf("%s\n", mes[0]);

    return 0;
}