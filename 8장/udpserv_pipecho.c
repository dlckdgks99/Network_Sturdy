//파이프를 통해 에코 메시지를 전달하는 UDP 에코 서버
//파이프(pipe) + 프로세스 분리(fork) + UDP 통신을 결합한 구조

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/types.h>
#include <errno.h>

#define MAX_BUFSZ 512

typedef struct mesg{
    struct sockaddr_in addr; //클라이언트의 IP/포트 정보
    char data[MAX_BUFSZ]; //클라이언트가 보낸 실제 메시지
} mesg_t;

void child_start(int sock, int pipefd[]);
void parent_start(int sock,int pipefd[]);
void errquit(char *mesg){perror(mesg); exit(1);}

int main(int argc, char **argv){
    struct sockaddr_in servaddr;
    pid_t pid;
    int sock, pipefd[2],port,len=sizeof(struct sockaddr);

    if(argc!=2){
        printf("\n Usage : %s port \n",argv[0]);
        exit(EXIT_FAILURE);
    }
    
    port=atoi(argv[1]);
    sock=socket(AF_INET,SOCK_DGRAM,0);//UDP 소켓 생성
    if(sock<0){
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    //서버 주소 구조체 설정
    bzero(&servaddr, len);
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=ntohs(port);
    bind(sock,(struct sockaddr*)&servaddr,len);

    //pipe 생성
    if(pipe(pipefd)==-1){
        errquit("pipe fail");
    }
    //fork로 프로세스 2개 생성
    pid=fork();
    if(pid<0){
        errquit("fork fail");
    }
    //부모
    else if(pid>0){
        parent_start(sock,pipefd);
    }

    //자식
    else if(pid==0){
        child_start(sock,pipefd);
    }
    return 0;
}

void child_start(int sock,int pipefd[]){//파이프에서 읽기만, 쓰지는않음
    mesg_t pmsg; //부모가 파이프로 보내준 데이터를 받을 버퍼.
    int nbytes=0, len=sizeof(struct sockaddr); // 읽은 바이트 수/보낸 바이트 수 저장, sendto에서 쓰는 주소 길이
    close(pipefd[1]); //자식프로세스는 파이프에 쓰기(write) 안함, 쓰기 끝을 닫아서 자식은 읽기 전용만으로 이용, 안 닫으면 EOF관련 문제 발생할 수도

    while(1){
        nbytes=read(pipefd[0],(char *)&pmsg, sizeof(mesg_t));//부모가 write한 mesg_t 구조체를 그대로 읽어옴, pipefd[0]에서 구조체 크기만큼 읽어서 pmsg에 저장
        if(nbytes<0){
            errquit("read failed");
        }
        printf("Child : %d read from pipe \n",nbytes);

        nbytes=sendto(sock,&pmsg.data,strlen(pmsg.data),0,(struct sockaddr*)&pmsg.addr,len);//UDP소켓으로 echo 응답 보내는 부분
        printf("Child : %d bytes echo response\n",nbytes);
        printf("----------------------------\n");
    }
}

void parent_start(int sock, int pipefd[]){//파이프에서 쓰기만, 읽지는 않음
    mesg_t pmsg; //파이프로 주고받을 구조체
    int nbytes, len=sizeof(struct sockaddr);

    
    close(pipefd[0]);// 부모 프로세스는 파이프를 읽지 않으므로 close(), 쓰기만 함
    printf("my echo server wait...\n");

    while(1){
        nbytes=recvfrom(sock,(void*)&pmsg.data,MAX_BUFSZ,0,(struct sockaddr*)&pmsg.addr,&len);//소켓으로부터 클라이언트 메시지 수신
        if(nbytes<0){
            errquit("recvfrom failed");
        }
        printf("Parent : %d bytes recv from socket\n",nbytes);
        pmsg.data[nbytes]=0;//문자열에 NULL 추가, recvfrom은 맨마지막에 널을 자동으로 안붙여주기에

        if(write(pipefd[1],(char*)&pmsg,sizeof(pmsg))<0){ //파이프의 쓰기 끝fd[1]에 pmsg구조체를 write()
            perror("write fail");
        }
        printf("Parent : %d write to pipe\n",nbytes);
    }
}