#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char *argv[]){
    struct sockaddr_in peer;
    int sock;
    int nbytes;
    char buf[512];

    if(argc !=3){
        printf("Usage : %s ip port\n",argv[0]);
        exit(1);
    }

    sock=socket(AF_INET,SOCK_DGRAM,0);
    bzero(&peer,sizeof(struct sockaddr));
    peer.sin_family=AF_INET;
    peer.sin_port=htons(atoi(argv[2]));
    peer.sin_addr.s_addr=inet_addr(argv[1]);

    while(fgets(buf,sizeof(buf),stdin)!=NULL){
        nbytes=sendto(sock,buf,strlen(buf),0,(struct sockaddr *)&peer,sizeof(peer));
        if(nbytes<0){perror("sendto fail"); exit(0);}
        nbytes=recvfrom(sock,buf,sizeof(buf)-1,0,0,0);
        if(nbytes<0){perror("recvfrom fail");exit(0);}
        buf[nbytes]=0;
        fputs(buf,stdout);
    }
    close(sock);
    exit(0);
}