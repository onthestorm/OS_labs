#include <sys/stat.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

int main() {
    int shm;
    char * str;

    shm = (shmget(3333, 128, 0666));

    if(shm == -1) {
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
        time_t m_time = time(NULL);
        printf("reciever time %.12s and pid %d\n", 4 + ctime(&m_time), getpid());
        printf("recieved :    %s\n\n", str);
        sleep(1);
        i++;
    }

    shmdt(str);

    return 0;
}

