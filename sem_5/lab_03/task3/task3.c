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
    char *com[N] = {"./uniq", "./matrix"};
    char * const argv[N][6] = {{com[0], "-c", "input.txt", "output.txt", NULL, NULL}, {com[1], "a", "matrix_1.txt", "matrix_2.txt", "matrix_output.txt", NULL}};
    
    printf("Parent process start! PID: %d, GROUP: %d\n", getpid(), getpgrp());

    for (int i = 0; i < N; i++)
    {
        int child_pid = fork();

        if(child_pid == -1)
        {
            perror("Error fork\n");
            exit(1);
        }
        else if (!child_pid)
        {
            printf("Child %d! PID: %d, PPID: %d, GROUP: %d\n", i + 1, getpid(), getppid(), getpgrp());
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
            child[i] = child_pid;
        }
    }

	for (int i = 0; i < N; i++)
	{
		int status;

		pid_t child_pid = wait(&status);

		printf("Child has finished: PID=%d. Status: %d\n", child_pid, status);

		if (WIFEXITED(status))
		{
			printf("Child process %d finished. Code: %d\n", child_pid, WEXITSTATUS(status));
		}
		else if (WIFSIGNALED(status))
		{
			printf("Child process %d finished from signal with code: %d\n", child_pid, WTERMSIG(status));
		}
		else if (WIFSTOPPED(status))
		{
			printf("Child process %d finished stopped. Number signal: %d\n", child_pid, WSTOPSIG(status));
		}
	}

    printf("Parent process finished! Children: %d, %d! \nParent: PID: %d, GROUP: %d\n ", child[0], child[1], getpid(), getpgrp());

    return 0;
}