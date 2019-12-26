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
    
    key_t key = ftok("key_t", 28);
    shm = shmget(key, 128, IPC_CREAT | 0660);

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
        char buf[9];
        struct tm * m_time;
        long int s_time = time(NULL);
        m_time = localtime(&s_time);
        strftime(buf, 9, "%X", m_time);
        sprintf(str, "%s from pid %d", buf, getpid());
        sleep(1);
        i++;
    }

    struct shmid_ds shmds;
    shmctl(shm, IPC_RMID, &shmds);

    shmdt(str);
    return 0;
}
