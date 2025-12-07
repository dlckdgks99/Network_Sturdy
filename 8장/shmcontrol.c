#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <time.h>

struct sembuf waitsem[]={{0,-1,0}};
struct sembuf notifysem[]={{0,+1,0}};

#define Semop(val) \
{if(semop val==-1) \
    errquit("semop fail");\
}

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short int *array;
    struct seminfo *__buf;
};

void errquit(char * msg){ perror(msg); exit(1);}
void fork_and_run();
void busy();
void access_shm();

char *shm_data;
int shmid, semid;

int main(int argc, char *argv[]) {
    key_t shmkey, semkey;
    unsigned short initsemval[1];

    if(argc < 3){
        printf("Usage : %s shmkey semkey\n", argv[0]);
        exit(1);
    }

    shmkey = atoi(argv[1]);
    semkey = atoi(argv[2]);

    shmid = shmget(shmkey, 128, IPC_CREAT | 0660);
    if(shmid < 0)
        errquit("shmget fail");

    shm_data = (char *)shmat(shmid, NULL, 0);
    if(shm_data == (char *)-1)
        errquit("shmat fail");

    semid = semget(semkey, 1, IPC_CREAT | 0660);
    if(semid < 0)
        errquit("semget fail");

    initsemval[0] = 1;

    union semun semarg;
    semarg.array = initsemval;

    if(semctl(semid, 0, SETALL, semarg) == -1)
        errquit("semctl");

    fork_and_run();
    fork_and_run();

    busy();

    wait(NULL);
    wait(NULL);

    shmctl(shmid, IPC_RMID, 0);
    semctl(semid, 0, IPC_RMID, 0);

    return 0;
}

void fork_and_run() {
    pid_t pid = fork();

    if(pid < 0)
        errquit("fork fail");

    if(pid == 0) {
        busy();
        exit(0);
    }
}

void busy() {
    for(int i=0;i<100;i++){
        Semop((semid,&waitsem[0],1));
        access_shm();
        Semop((semid,&notifysem[0],1));
    }
    shmdt(shm_data);
}

void access_shm() {
    struct timespec ts = {0, 100000000};

    sprintf(shm_data, "%d", getpid());

    for(int i=0;i<1000;i++); // delay

    int pid = atoi(shm_data);

    if(pid != getpid())
        puts("Error : 다른 프로세스도 동시에 공유메모리 접근함");
    else
        putchar('.');

    fflush(stdout);
    nanosleep(&ts, NULL);
}
