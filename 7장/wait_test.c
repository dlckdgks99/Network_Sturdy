//wait() 함수 동작의 이해

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

// 사용자 시그널 핸들러 함수
void catch_sigchld(int signo){
    puts("###(Parent) catch SIGCHLD");
}

int chstat;
int main(int argc, char *argv[]){
    int i,n;
    struct sigaction sact;
    sact.sa_flags=0;
    sigemptyset(&sact.sa_mask);
    sigaddset(&sact.sa_mask,SIGCHLD);


    //자식이 죽어서 SIGCHLD가 왔을 때 어떻게 처리할지
    //sact.sa_handler=SIG_DFL;  //기본동작(시그널 무시)
    sact.sa_handler=SIG_IGN; // 시그널 무시(SIG_IGN)를 등록
    //sact.sa_handler=catch_sigchld; // SIGCHLD 시그널 핸들러를 등록
    sigaction(SIGCHLD,&sact,NULL);


    //자식 5개 생성
    for(i=0; i<5; i++){
        if(fork()==0){
            if(i>2){
                sleep(6);
                printf("(%d번 Child), PID=%d, PPID=%d Exited\n",i,getpid(),getppid());
                exit(13);
            }
        }
    }

    sleep(3);
    puts("--------------");
    system("ps -e -o pid,ppid,stat,cmd | grep wait_test");
    puts("--------------");

    //부모의 wait() 루프
    puts("#(Parent) wait 호출함");
    for(;;){
        chstat=-1;
        n=wait(&chstat);
        printf("# wait =%d(child stat=%d)\n",n,chstat);
        if(n==-1){
            if(errno==ECHILD){
                perror("기다릴 자식프로세스가 존재하지 않음");
                break;
            }
            else if(errno==EINTR){
                perror("wait 시스템 콜이 인터럽트 됨"); continue;
            }
        }
    }
    puts("# (Parent) 종료함");
    return 0;
}