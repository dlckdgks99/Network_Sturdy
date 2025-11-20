#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

/*
-servent의 구조체
struct servent {
    char *s_name;   //서비스 이름
    char **s_aliases;   //별명 목록
    int s_port; //포트번호
    char *s_proto;  //사용하는 프로토콜
}

*/

int main(void){
    struct servent *servent;
    servent = getservbyname("echo","udp"); //
    if(servent==NULL){
        printf("서비스 정보를 얻을 수 없음. \n\n");
        exit(0);
    }
    printf("UDP 에코 s_name : %s\n", servent->s_name);
    printf("UDP 에코 포트번호 (네트워크 순서) : %d \n", servent->s_port);
    printf("UDP 에코 포트번호(호스트 순서) : %d \n",ntohs(servent->s_port));


    return 0;
}



/*
UDP 에코 포트번호 (네트워크 순서) : 1792 
UDP 에코 포트번호(호스트 순서) : 7 
인텔 80x86 계열의 CPU를 사용하는 컴퓨터(PC)에서의 실행 결과
*/