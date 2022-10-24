#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    int childpid;
    if ((childpid = fork()) == -1) {
        perror("Can’t fork.\n");
        return 1;
    } else if (childpid == 0) {
        while (1) {
            printf("Child %d \n", getpid());
        }
        return 0;
    } else {
        while (1) {
            printf("Parent %d  \n", getpid());
        }
        return 0;
    }
}