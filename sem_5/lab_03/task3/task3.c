#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

#define N 2
#define TIME_SLEEP 2

int main()
{
    setbuf(stdout, NULL);
    int child[N];
    char *com[N] = {"./matrix", "./uniq",};
    char * const argv[N][5] = {{com[0], "a", "matrix_1.txt", "matrix_2.txt", NULL}, {com[1], "-c", "input.txt", "output.txt", NULL},};
    
    printf("Parent process start! PID: %d, GROUP: %d\n", getpid(), getpgrp());

    for (int i = 0; i < N; i++)
    {
        child[i] = fork();

        if(child[i] == -1)
        {
            perror("Error fork\n");
            exit(1);
        }
        else if (!child[i])
        {
            printf("Child: PID: %d, PPID: %d, GROUP: %d\n", getpid(), getppid(), getpgrp());
            int rc = execv(com[i], argv[i]);

            if (rc == -1)
            {
                perror("Exec error");
                exit(1);
            }

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

    return 0;
}