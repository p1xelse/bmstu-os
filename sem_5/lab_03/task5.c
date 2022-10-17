#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdbool.h> 
#include <signal.h>
#include <stdlib.h> 

#define N 2
#define TIME_SLEEP 2

_Bool flag = false;


void catch_sig(int sig_num)
{
	flag = true;
	printf("catch_sig: %d\n", sig_num);
}

int main()
{
    int child[N];
    int fd[N];
    char buf[64] = { 0 };
    char *mes[N] = {"Hello from process 1\n", "Hello from process 2\n"};

    if (pipe(fd) == -1)
    {
        perror("Can't pipe!");
        exit(1);
    }

    printf("Parent process start! PID: %d, GROUP: %d\n", getpid(), getpgrp());
    signal(SIGINT, catch_sig);
    sleep(2);

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
            if (flag)
            {
                close(fd[0]);
                write(fd[1], mes[i], strlen(mes[i]));
                printf("Message %d sent to parent! %s", i + 1, mes[i]);
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

    
    printf("\nMessage receive :\n");
    close(fd[1]);
    read(fd[0], buf, 64);
    printf("%s\n", buf);
    

    printf("Parent process finished! Children: %d, %d! \nParent: PID: %d, GROUP: %d\n", child[0], child[1], getpid(), getpgrp());

    return 0;
}