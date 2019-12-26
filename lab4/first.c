#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

int main() {
    int shm;
    char * str;

    shm = (shmget(3333, 128, IPC_CREAT | 0660));

    if(shm == -1) {
        perror("first : shm not created");
        exit(1);
    }

    str = shmat( shm, NULL, 0 );
    if(str == (char*)-1) {
        perror("shmat error");
        exit(1);
    }

    int i = 0;
    while(i < 30) {
        time_t m_time = time(NULL);
        sprintf(str, "%.12s from pid %d", 4 + ctime(&m_time), getpid());
        i++;
    }

    shmdt(str);
    return 0;
}
