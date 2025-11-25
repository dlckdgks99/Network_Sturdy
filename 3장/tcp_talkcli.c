#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


#define MAXLINE 511

char *EXIT_STRING ="exit";
int recv_and_print(int sd);
int input_and_send(int sd);

int main(int argc, char *argv[]){
    pid_t pid;
    static int s;
    static struct sockaddr_in servaddr;

    if (argc!= 3){
        printf("사용법 :%s server_ip port \n", argv[0]);
        exit(0);
    }

    if((s=socket(PF_INET,SOCK_STREAM,0))<0){
        printf("Client : Can't open stream socket. \n");
        exit(0);
    }

    bzero((char *) & servaddr, sizeof(servaddr));

    servaddr.sin_family=AF_INET;
    inet_pton(AF_INET,argv[1],&servaddr.sin_addr); //사람이 읽을 수 있는 IP 문자열(text) → 네트워크용 이진(binary) 주소로 변환
    servaddr.sin_port=htons(atoi(argv[2])); //프로그램 실행 시 전달된 두 번째 인자를 atoi를 통해 int 정수로 변환시키고 htons를 통해  호스트바이트 순서 -> 네트워크 바이트 순서로 변환 
    
    //서버에 연결요청
    if(connect(s,(struct sockaddr * )&servaddr,sizeof(servaddr))<0){
        printf("Client: can't connect to server.\n");
        exit(0);
    }
    if((pid=fork())>0){
        input_and_send(s);
    }                                                                                                                                                       
    else if(pid==0){
        recv_and_print(s);
    }
    close(s);
    return 0;
}

int input_and_send(int sd){
    char buf[MAXLINE+1];
    int nbyte;
    while(fgets(buf,sizeof(buf),stdin) != NULL){
        nbyte=strlen(buf);
        write(sd,buf,strlen(buf));
        if(strstr(buf,EXIT_STRING) !=NULL){
            puts("Good bye.");
            close(sd);
            exit(0);
        }
    }
    return 0;
}

int recv_and_print(int sd){
    char buf[MAXLINE+1];
    int nbyte;
    while(1){
        if((nbyte=read(sd,buf,MAXLINE))<0){
            perror("read fail");
            close(sd);
            exit(0);
        }
        buf[nbyte]=0;
        if(strstr(buf,EXIT_STRING) !=NULL){
            break;
        }
        printf("client : %s",buf);
    }
    return 0;
}