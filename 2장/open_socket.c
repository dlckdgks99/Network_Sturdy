#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>



/*
int socket(
    int domain, //프로토콜체계
    int type,   //서비스 타입
    int protocol    //소켓에서 사용할 프로토콜
);
*/

int main(){
    // 한프로세스에서 개설 가능한 최대 소켓 수를 찾는
    printf("getdtablesize() =%d\n",getdtablesize());

    //파일 및 소켓 번호
    int fd1, fd2, sd1, sd2;

    //파일 열기
    fd1=open("/etc/passwd", O_RDONLY,0);
    printf("/etc/passwd/s file descroptor = %d\n", fd1);

    //스트림형 소켓 열기
    sd1= socket(PF_INET, SOCK_STREAM,0);
    printf("datagram socket descriptor=%d\n",sd1);

    sd2=socket(PF_INET,SOCK_DGRAM,0);
    printf("datagram socket descriptor = %d\n",sd2);

    //또 다른 파일 열기
    fd2 = open("/etc/hosts",O_RDONLY,0);
    printf("/etc/hosts's file descriptor = %d\n",fd2);

    close(fd2);
    close(fd1);
    close(sd1);
    close(sd1);
    return 0;
}

/* 출력 결과
/etc/passwd/s file descroptor = 3
datagram socket descriptor=4
datagram socket descriptor = 5
/etc/hosts's file descriptor = 6
*/
