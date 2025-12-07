#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main(){
    int s;
    int val,len;

    if((s=socket(AF_INET,SOCK_STREAM,0))<0){
        perror("socket fail");
        exit(1);
    }
    len=sizeof(val);
    printf("%d",len);
    if(getsockopt(s,SOL_SOCKET,SO_RCVBUF,&val,&len)<0){ //getsockopt() 는 &val 에 "값을 넣어서 돌려주는" 함수
        perror("socket fail");
        exit(1);
    }
    printf("디폴트 수신버퍼 크기 :%d\n",val);
    val=1024;
    setsockopt(s,SOL_SOCKET,SO_RCVBUF,&val,sizeof(val)); //setsockopt() 는 val 값을 읽어서 커널 설정을 변경하는 함수, &val이 가리키는 값(1024)을 읽어서 커널의 소켓 수신 버퍼 크기를 변경
    getsockopt(s,SOL_SOCKET,SO_RCVBUF,&val,&len);
    printf("1024로 변경한 수신버퍼 크기 :%d\n",val);
    return 0;
}

/*
### getsockopt()
extern int getsockopt(int fd, int level, int optname, void *optval, socklen_t *optlen);

- fd: 옵션을 조회할 소켓 번호
- level: 옵션 레벨 (SOL_SOCKET, IPPROTO_IP, IPPROTO_TCP 등)
- optname: 조회할 옵션 이름
- optval: 옵션 값을 저장할 버퍼(포인터)
- optlen: optval의 크기를 나타내는 변수(입력 + 출력)

### setsockopt()
extern int setsockopt(int fd, int level, int optname, const void *optval, socklen_t optlen);

- fd: 옵션을 설정할 소켓 번호
- level: 옵션 레벨 (SOL_SOCKET, IPPROTO_IP, IPPROTO_TCP 등)
- optname: 설정할 옵션 이름
- optval: 설정할 옵션 값이 저장된 버퍼(포인터)
- optlen: optval의 크기(바이트 단위)


*/