//메시지큐에서 에코 메시지를 읽어서 응답해줌

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MAX_BUFSZ 512

typedef struct _msg{
    long msg_type;
    struct sockaddr_in addr;
    char msg_text[MAX_BUFSZ];
} msg_t;

//메시지큐에서 읽어서 응답하기
int qread_and_echoreply(int msqid,int sock);
void errquit(char *mesg){perror(mesg); exit(1);}

int main(int argc,char **argv){
    struct sockaddr_in servaddr;
    key_t key;
    int sock,port,msqid,len=sizeof(struct sockaddr);

    key=atoi(argv[1]);
    port=atoi(argv[2]);
    if((msqid=msgget(key,0))==-1){
        errquit("rep msgget fail");
    }
    sock=socket(AF_INET,SOCK_DGRAM,0);
    bzero(&servaddr,len);
    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    servaddr.sin_port=htons(port);
    bind(sock,(struct sockaddr*)&servaddr,len);

    qread_and_echoreply(msqid,sock);
    return 0;
}

int qread_and_echoreply(int msqid,int sock){
    int size, len=sizeof(struct sockaddr);
    msg_t pmsg;
    size=sizeof(msg_t)-sizeof(long);
    pmsg.msg_type=0;
    for(;;){
        if(msgrcv(msqid,(void *)&pmsg,size,0,0)<0){
            errquit("msgrcv fail");
        }
        printf("reply process PID=%d\n",getpid());
        printf("%s",pmsg.msg_text);
        if(sendto(sock,pmsg.msg_text,strlen(pmsg.msg_text),0,(struct sockaddr*)&pmsg.addr,len)<0){
            errquit("serv2 sendto fail");
        }
        pmsg.msg_text[0]=0;
    }
}