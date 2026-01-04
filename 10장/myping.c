//신뢰성(재전송·순서 보장)이 없는 환경에서 네트워크 상태를 빠르게 확인하기 위해 ICMP 프로토콜을 이용해 ping을 직접 구현

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/in.h>

#define BUFSIZE 4096

int seqnum;
char recvbuf[BUFSIZE];
char sendbuf[BUFSIZE];
int rawsock;
int notrecv=0;
struct sockaddr_in sendaddr, recvaddr;

int send_ping();
int prn_rcvping(char * ipdata, int recvsize);
void prn_icmp(struct icmphdr *icmp, int icmpsize);
unsigned short in_cksum(unsigned short *addr,int len);
void errquit(char *msg){perror(msg);exit(0);}

int main(int argc, char **argv){
    int recvsize, addrlen=sizeof(struct sockaddr);
    fd_set readset;
    struct timeval tv;
    int ret;

    if(argc !=2){
        printf("Usage: %s ip_addressn",argv[0]);
        exit(1);
    }

    addrlen=sizeof(struct sockaddr);
    bzero(&recvaddr, sizeof(struct sockaddr));
    bzero(&sendaddr, sizeof(struct sockaddr));

    // 목적지 IP 설정 ICMP는 포트개념이 없어서 0으로 설정
    sendaddr.sin_family=AF_INET;
    inet_pton(AF_INET,argv[1],&sendaddr.sin_addr.s_addr);
    sendaddr.sin_port=htons(0);

    //raw socket 생성 + connect
    rawsock=socket(AF_INET,SOCK_RAW,IPPROTO_ICMP);
    if(rawsock<0){
        errquit("socket fail");
    }
    if(connect(rawsock,(struct sockaddr *)&sendaddr,sizeof(struct sockaddr))!=0){
        errquit("connect fail");
    }

    //1초마다 ping 보내고, 1초 기다리고, 오면 출력
    while(1){
        FD_ZERO(&readset);
        FD_SET(rawsock,&readset);
        tv.tv_sec=1;
        tv.tv_usec=0;
        send_ping();
        ret=select(rawsock+1,&readset,NULL,NULL,&tv);
        if(ret==0){
            if(++notrecv==3){
                notrecv=0;
                puts("REQUEST TIMEOUT");
            }
            continue;
        }
        else if(ret<0){
            errquit("select fail");
        }
        recvsize=recvfrom(rawsock,recvbuf,sizeof(recvbuf),0,(struct sockaddr*)&recvaddr,&addrlen);
        if(recvsize<0){
            errquit("recvfrom fail");
        }
        notrecv=0;
        prn_rcvping(recvbuf,recvsize);
        sleep(1);
    }
    exit(0);
}

//prn_icmp(): ICMP 헤더 정보 출력   
void prn_icmp(struct icmphdr * icmp,int icmpsize){
    printf("[icmp](id:%d",icmp->un.echo.id);
    printf("seq:%d",icmp->un.echo.sequence);
    printf("code:%d",icmp->code);
    printf("type:%d\n",icmp->type);
}

//prn_rcvping(): 수신 패킷에서 IP/ICMP 꺼내서 Echo Reply만 출력
int prn_rcvping(char *ipdata,int recvsize){
    int ip_headlen,icmp_len;
    struct icmphdr * icmp;
    struct iphdr *ip;
    char buf[512];

    ip=(struct iphdr*)ipdata;
    ip_headlen=ip->ihl*4;
    icmp_len=recvsize - ip_headlen;
    icmp=(struct icmphdr *) (ipdata + ip_headlen);
    if(icmp->type != ICMP_ECHOREPLY){
        return -1;
    }
    inet_ntop(AF_INET,(void*)&ip->saddr, buf, sizeof(buf));
    printf("%d bytes recv from (%s)", icmp_len,buf);
    prn_icmp(icmp,icmp_len);
    return 0;
}

//send_ping(): ICMP Echo Request 만들어서 전송
int send_ping(){
    struct icmphdr *icmp;
    int len,sendsize;
    icmp=(struct icmphdr *) sendbuf;
    bzero((char *)icmp, sizeof(struct icmp));
    icmp->code=0;
    icmp->type = ICMP_ECHO;
    icmp->un.echo.sequence=seqnum++;
    icmp->un.echo.id=getpid();
    icmp-> checksum=0;
    icmp->checksum=in_cksum((unsigned short *)icmp, sizeof(struct icmp));
    len=sizeof(struct icmphdr);
    sendsize=sendto(rawsock,sendbuf,len,0,(struct sockaddr*)&sendaddr, sizeof(struct sockaddr));
    prn_icmp(icmp,sendsize);
    return sendsize;

}


//in_cksum(): 체크섬 계산
unsigned short in_cksum(unsigned short*addr,int len){
    int nleft=len;
    int sum=0;
    unsigned short *w=addr;
    unsigned short answer=0;

    while(nleft>1){
        sum+=*w++;nleft-=2;
    }
    if(nleft==-1){
        *(unsigned char*)(&answer)=*(unsigned char*)w;
        sum+=answer;
    }
    sum=(sum>>16)+(sum & 0xffff);
    sum+=(sum>>16);
    answer=~sum;
    return (answer);
}