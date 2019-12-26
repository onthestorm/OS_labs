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

            char readed[80];
            int fd = open("FIFO", O_RDONLY);

            if(fd == -1) {
                perror("CHILD: Can't open FIFO\n");
                exit(0);
            }

            read(fd, &readed, 80);
            printf("READED FROM FD : %s\n", readed);

            char buf[9];

            s_time = time(NULL);
            m_time = localtime(&s_time);
            strftime(buf, 9, "%X", m_time);

            printf("Time in child procces with pid %d with parent pid %d is %s\n", getpid(), getppid(), buf);
            close(fd);
            exit(exit_code);
        }

        default: {
            char str[80];
            char buf[9];

            s_time = time(NULL);
            m_time = localtime(&s_time);
            strftime(buf, 9, "%X", m_time);

            sprintf(str, "Time in parent procces with pid %d is %s", getpid(), buf);
            
            printf("parent sending : %s\n", str);

            int fd = open("FIFO", O_WRONLY);
            if(fd == -1) {
                perror("PARENT: Can't open FIFO\n");
                exit(0);
            }
            write(fd, str, 80);
            waitpid(pid, &exit_code, 0);
            close(fd);
        }
    }
    unlink("FIFO");
    return 0;
};
