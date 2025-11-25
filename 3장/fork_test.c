#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int global_var=0; //전역 변수 선언

int main(void){
    pid_t pid;  
    int local_var =0; 
    if ((pid=fork())<0){
        printf("fork error\n");
        exit(0);
    }
    else if(pid==0){
        global_var++;
        local_var++;
        printf("CHILD - my pid is %d and parent's pid is %d\n",getpid(),getppid());
    }
    else{
        //부모 프로세스
        sleep(2); //2초 쉰다
        global_var +=5;
        local_var+=5;
        printf("PARENT - my pid is %d, child'pid is %d\n",getpid(),pid);
    }
    printf("\t global var : %d\n",global_var);
    printf("\t local var : %d\n",local_var);
}
// getpid()는 자신의 PID를 얻고, getppid()는 부모 프로세스의 PID를 얻는다
// pid_t 프로세스 ID를 저장하기 위한 정수 타입, typedef로 정의된 정수형(type)