#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ipc.h> //System V IPC(공유메모리, 메시지큐, 세마포어 등)에서 사용하는 key_t 타입, IPC 관련 상수 정의.
#include <sys/shm.h> //공유 메모리(shmget, shmat, shmdt, shmctl 등)를 사용하기 위한 헤더
#include <sys/types.h>
#include <sys/wait.h>//wait, waitpid 함수 사용을 위한 헤더.

void errquit(char *msg){ perror(msg); exit(1); }
void fork_and_run();
void busy();
void access_shm(int count);
char *shm_data; //공유 메모리 영역의 시작 주소를 가리키는 포인터
int shmid; //공유 메모리 segment의 ID

int main(int argc, char*argv[]){
    key_t shmkey;

    if(argc < 2){
        printf("Usage : %s shmkey\n", argv[0]);
        exit(1);
    }

    shmkey = atoi(argv[1]);
    shmid = shmget(shmkey, 128, IPC_CREAT | 0660); // 공유 메모리 segment 생성 또는 얻기, IPC_CREAT: 없으면 새로 생성

    if(shmid < 0)
        errquit("shmget fail");

    shm_data = (char*)shmat(shmid, (void*)0, 0); // 공유 메모리 segment를 현재 프로세스 주소공간에 붙임(attach).
                                                 // shmat(shmid, 붙일 주소(0이면 커널이 적당한 주소 선택), flag (0이면 read/write로 attach)), 반환값: 공유메모리가 매핑된 시작 주소 (void*) → 이를 char*로 캐스팅해서 문자열처럼 사용.
    if(shm_data == (char*)-1)
        errquit("shmat fail");

    fork_and_run(); // 자식 1
    fork_and_run(); // 자식 2

    busy();         // 부모도 busy()

    wait(NULL);
    wait(NULL);

    shmctl(shmid, IPC_RMID, 0); //공유메모리 segment를 시스템에서 제거
    return 0;
}

//자식 프로세스를 만들고, 자식에게 busy()를 시키는 함수.
//부모는 fork_and_run을 두 번 호출만 할 뿐, 그 안에서 busy()를 하진 않고 main에서 busy() 호출.
void fork_and_run(){
    pid_t pid = fork(); 

    if(pid < 0){
        errquit("fork fail");
    }
    else if(pid == 0){
        busy();
        exit(0);
    }
}

//공유메모리에 반복적으로 접근하는 "부하용" 함수
void busy(){
    for(int i = 0; i < 500000; i++){
        access_shm(i); //매 반복마다 공유메모리에 접근하는 함수 호출
    }
    shmdt(shm_data);//이 프로세스가 더 이상 공유메모리를 사용하지 않겠다고 detach. 해당 프로세스의 주소공간에서 공유메모리 매핑 해제.
}

//count는 busy() 반복 횟수(몇 번째 접근인지 알려줌)
void access_shm(int count){
    int i;
    pid_t pid;

    sprintf(shm_data, "%d", getpid()); //shm_data가  shmat()으로 attach한  공유메모리의 실제 주소

    for(i = 0; i < 1000; i++);  // delay

    pid = atoi(shm_data);

    if(pid != getpid()){
        printf("Error(count=%d) : 다른 프로세스도 동시에 공유메모리 접근함\n", count);
    }
}
