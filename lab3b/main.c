#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

int main() {
    pid_t pid;

    if(mkfifo("FIFO", 0666) == -1) {
        perror("error fifo");
        return 1;
    }

    int exit_code = 0;
    struct tm * m_time;
    long int s_time;

    switch(pid = fork()) {
        case -1:
            exit(1);
        case 0: {
            sleep(1);

            char readed[64];
            int fd = open("FIFO", O_RDONLY);

            if(fd == -1) {
                perror("CHILD: Can't open FIFO\n");
                exit(0);
            }

            read(fd, &readed, 64);
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
            close(fd);
            exit(exit_code);
        }

        default: {
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
            
            printf("parent sending : %s\n", buf);

            int fd = open("FIFO", O_WRONLY);
            if(fd == -1) {
                perror("PARENT: Can't open FIFO\n");
                exit(0);
            }
            write(fd, str, 64);
            waitpid(pid, &exit_code, 0);
            close(fd);
        }
    }
    unlink("FIFO");
    return 0;
};
