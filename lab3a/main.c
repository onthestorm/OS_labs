#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

int main() {
    pid_t pid;
    int fd[2];

    if(pipe(fd) < 0)
        exit(1);

    int exit_code = 0;
    struct tm * m_time;
    long int s_time;

    switch(pid = fork()) {
        case -1:
            exit(1);
        case 0: {
            close(fd[1]);
            sleep(1);
            char readed[64];

            read(fd[0], &readed, 64);
            printf("READED FROM FD : %s\n", readed);

            char str[64] ={"Time in child procces with pid "};
            char buf[9];

            s_time = time(NULL);
            m_time = localtime(&s_time);
            strftime(buf, 9, "%X", m_time);

            char * mypid = malloc(sizeof(pid_t) + 1);
            sprintf(mypid, "%d", getpid());
            char * myppid = malloc(sizeof(pid_t) + 1);
            sprintf(myppid, "%d", getppid());

            strcat(str, mypid);
            strcat(str, " with parent pid ");
            strcat(str, myppid);
            strcat(str, " is ");
            strcat(str, buf);
            printf("%s\n", str);
            close(fd[0]);
            exit(exit_code);
        }

        default: {
            close(fd[0]);
            char str[64] ={"Time in parent procces with pid "};
            char buf[9];

            s_time = time(NULL);
            m_time = localtime(&s_time);
            strftime(buf, 9, "%X", m_time);

            char * mypid = malloc(sizeof(pid_t) + 1);
            sprintf(mypid, "%d", getpid());

            strcat(str, mypid);
            strcat(str, " is ");
            strcat(str, buf);

            printf("parent sending: %s\n", str);

            write(fd[1], str, 64);
            waitpid(pid, &exit_code, 0);
            close(fd[1]);
        }
    }
    return 0;
};
