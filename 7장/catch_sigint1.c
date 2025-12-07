
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

int count;


//시그널 핸들러 정의
void catch_sigint(int signum){
    printf("\n(count=%d) CTRL-C pressed!\n",count);
    return;
}

int main(int argc, char *argv[]){

    //SIGACTION 구조체 설정
    struct sigaction act;
    sigset_t masksets; // 모든 시그널을 포함한 시그널의 집합
    int i;
    char buf[10];
    sigfillset(&masksets); //모든 시그널을 block

    act.sa_handler=catch_sigint; //아까 만든 cat_sigint 라는 함수 시그널 핸들러를 입력
    act.sa_mask=masksets; //모든 시그널 블록
    act.sa_flags=0; // 옵션 없음
    sigaction(SIGINT,&act,NULL); //SIGINT의 기본 동작을 내가 지정한 핸들러(catch_sigint)로 바꿈
    for(count=0; count<3;count++){
        read(0,buf,sizeof(buf));
    }
    return 0;
}

//sigaction 구조체 값들
/*struct sigaction {
	__sighandler_t sa_handler;
	unsigned long sa_flags;
	__sigrestore_t sa_restorer;
	sigset_t sa_mask;		
}; */