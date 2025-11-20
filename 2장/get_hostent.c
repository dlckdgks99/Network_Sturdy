
// 도메인 이름을 IP 주소로 변환
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>

/*
Description of data base entry for a single host.  
struct hostent
{
  char *h_name;			//호스트 이름
  char **h_aliases;		//호스트 별명들
  int h_addrtype;		//호스트 주소의 종류(AF_INET=2등)
  int h_length;			//주소의 크기 (바이트 단위이며 IPv4에서는 4임)
  char **h_addr_list;	//IP 주소 리스트	
#ifdef __USE_MISC
# define	h_addr	h_addr_list[0] 
}; */

//gethostbyname()은 IP 주소를 포함하고 있는 구조체 in_addr의 포인터와 이 주소의 길이, 주소타입을 입력하여 해당 호스트의 정보를 가지고 있는 hostent 구조체의 포인터를 리턴
int main(int argc, char *argv[]){
    struct hostent *hp;
    struct in_addr in;
    int i;
    char buf[20];
    if(argc<2){
        printf("Usage:%s hostname\n",argv[0]);
        exit(1);
    }
    hp=gethostbyname(argv[1]);
    if(hp==NULL){
        printf("gethostbyname fail \n");
        exit(0);
    }
    printf("호스트 이름     :%s\n",hp->h_name);
    printf("호스트 주소타입 번호:%d\n",hp->h_addrtype);
    printf("호스트 주소의 길이  :%d 바이트\n",hp->h_length);
    for (i=0; hp->h_addr_list[i];i++){
        memcpy(&in.s_addr,hp->h_addr_list[i],sizeof(in.s_addr));
        inet_ntop(AF_INET,&in,buf,sizeof(buf));
        printf("IP주소(%d 번째) : %s\n",i+1,buf);
    }
    for(i=0;hp->h_aliases[i];i++){
        printf("호스트 별명(%d 번째) : %s",i+1,hp->h_addr_list[i]);
    }
    puts("");
    return 0;
}