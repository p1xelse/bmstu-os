#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 2
#define TIME_SLEEP 2

int main()
{
    int child[N];

    printf("Parent process start! PID: %d, GROUP: %d\n", getpid(), getpgrp());

    for (int i = 0; i < N; i++)
    {
        child[i] = fork();

        if(child[i] == -1)
        {
            perror("Fork error()\n");
            exit(1);
        }
        else if (!child[i])
        {
            printf("BEFORE SLEEP Child: PID: %d, PPID: %d, GROUP: %d \n", getpid(), getppid(), getpgrp());

            sleep(TIME_SLEEP);
            printf("AFTER SLEEP Child: PID: %d, PPID: %d, GROUP: %d\n", getpid(), getppid(), getpgrp());
            return 0;
        }
        else
        {
            printf("Parent process: PID=%d; GROUP: %d, Child: PID=%d\n", getpid(), getpgrp(), child[i]);
        }
        
    }

    return 0;
}