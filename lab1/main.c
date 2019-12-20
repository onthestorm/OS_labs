#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

void pid_atexit();

int main() {
    pid_t pid;
    int exit_code;

    atexit(pid_atexit);

    switch (pid = fork()) {
        case -1:
            exit(1);
        case 0:
            printf("This is child procces with pid = %d, with parent pid = %d\n", getpid(), getppid());
            exit(exit_code);
        default:
            printf("This is parent procces with pid = %d, with child procces pid = %d\n", getpid(), pid);
            waitpid(pid, &exit_code, 0);
    }

    return 0;
}

void pid_atexit() {
    printf("atexit worked for pid:%d\n", getpid());
}
