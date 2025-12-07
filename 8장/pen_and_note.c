//공유 데이터의 경쟁적 접근을 세마포어로 제어
//공유 자원(연필 PEN, 노트 NOTE)을 여러 프로세스가 동시에 사용하려고 할 때 경쟁적 접근을 세마포어로 제어하는 예제

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/sem.h>

void errquit(char *mesg){
    perror(mesg);
    exit(0);
}

#define PEN 0
#define NOTE 1

struct sembuf increase[]={
    {0,+1,SEM_UNDO},{1,+1,SEM_UNDO} //{세마포어 번호 0(PEN)에 대해 값 +1 (자원 반환), SEM_UNDO 옵션 | 세마포어 번호 1(NOTE)에 대해 값 +1}
};
struct sembuf decrease[]={
    {0,-1,SEM_UNDO},{1,-1,SEM_UNDO}
};

unsigned short seminitval[]={1,2}; //세마포어 초기값 배열

union semun{
    int val;
    struct semid_ds *buf;
    unsigned short int *array;
    struct seminfo *__buf;
} semarg;

int semid;
void do_work();

int main(int argc, char * argv[]){
    semid=semget(0x1234,2,IPC_CREAT | 06600);  //세마포어 생성
    if(semid==-1){
        semid=semget(0x1234,0,0); //이미 존재하는 세마포어 집합에 그냥 접근만 시도
    }
    semarg.array=seminitval; //semarg.array 필드에 초기값 배열 seminitval의 주소를 넣음
    if(semctl(semid,0,SETALL,semarg)==-1){ //세마포어 집합 전체 값 초기화.
        errquit("semctl");
    }
    setvbuf(stdout,NULL,_IONBF,0);//stdout을 버퍼링 없이 사용하도록 설정 -> printf 할 때마다 즉시 출력되도록 하기 위해
    
    //fork() 2번으로 4개의 프로세스 생성
    fork(); 
    fork();
    do_work();//fork로 생긴 모든 프로세스가 공통으로 이 함수를 실행. 즉, 4개의 프로세스가 동시에 do_work()로 들어감.
    semctl(semid,0,IPC_RMID,0); //세마포어 제어함수, 세마포어 집합 전체 값 초기화.
    return 0;
}

void do_work(){
    int count=0;
    #define Semop(val) if((semop val)==-1) errquit("semop") //매크로 정의 
    while(count<3){
        Semop((semid,&decrease[PEN],1));
        printf("[pid : %5d]연필을 들고 \n",getpid());
        Semop((semid,&decrease[NOTE],1));
        printf("\t[pid:%5d]노트를 들고\n",getpid());

        Semop((semid,&increase[PEN],1));
        printf("\t[pid:%5d]연필을 내려놓음\n",getpid());
        Semop((semid,&increase[NOTE],1));
        printf("\t[pid:%5d]노트를 내려놓음\n",getpid());

        sleep(1);
        count++;
    }
}