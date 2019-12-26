#include <sys/stat.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <errno.h>


int main() {
    int shm;
    char * str;
    
    sleep(1);
    key_t key = ftok("./key_t", 28);

    if(key == -1)
    {
        perror("key");
        return EXIT_FAILURE;
    }

    shm = shmget(key, 128, 0);

    if(shm == -1) {
        printf("%d", errno);
        perror("second : shm not created");
        exit(1);
    }

    sleep(1);

    str = shmat( shm, NULL, 0 );
    if(str == (char*)-1) {
        perror("shmat error");
        exit(1);
    }

    sleep(1);

    int i = 0;
    while(i < 30) {
        char buf[9];
        struct tm * m_time;
        long int s_time = time(NULL);
        m_time = localtime(&s_time);
        strftime(buf, 9, "%X", m_time);

        printf("reciever time %s and pid %d\n", buf, getpid());
        printf("recieved :    %s\n\n", str);
        sleep(1);
        i++;
    }

    shmdt(str);

    return 0;
}

