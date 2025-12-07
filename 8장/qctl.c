// msgctl()를 이용해 메시큐의 정보를 출력

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int msq_remove(int qid);
int view_qinfo(int qid);

int main(int argc, char **argv){
    int qid;
    key_t key;
    if(argc!=2){
        printf("USAGE : %s msqkey\n",argv[0]);
        exit(EXIT_FAILURE);
    }
    key=atoi(argv[1]);

    if((qid=msgget(key,0))<0) {//msgget()로 메시지큐 열기
        perror("msgget fail");
        exit(0);
    }

    view_qinfo(qid);
    msq_remove(qid);
    exit(EXIT_SUCCESS);

}

// 메시지큐 정보 출력 함수
int view_qinfo(int qid){
    struct msqid_ds buf;
    struct ipc_perm *pm;

    if ((msgctl(qid,IPC_STAT,&buf))<0){ //커널이 가지고 있는 메시지큐 상태 정보를 buf에 복사해라.
        perror("msgctl");
        return -1;
    }

    pm=&buf.msg_perm;
    printf("큐의 최대 바이트수: %ld\n",buf.msg_qbytes);
    printf("큐의 유효 사용자 UID : %d\n",pm->uid);
    printf("큐의 유효 사용자 : %d\n",pm->gid);
    printf("큐 접근권한 : 0%o\n",pm->mode);
    return 0;
}

int msq_remove(int qid){
    if((msgctl(qid,IPC_RMID,NULL))<0){
        perror("msgctl");
        return -1;
    }
    printf("메시지큐 %d 삭제됨 \n",qid);
    return 0;
}




/*
struct msqid_ds
{
  struct ipc_perm msg_perm;	/* structure describing operation permission 
  __MSQ_PAD_TIME (msg_stime, 1);	/* time of last msgsnd command 
  __MSQ_PAD_TIME (msg_rtime, 2);	/* time of last msgrcv command 
  __MSQ_PAD_TIME (msg_ctime, 3);	/* time of last change 
  __syscall_ulong_t __msg_cbytes; /* current number of bytes on queue 
  msgqnum_t msg_qnum;		/* number of messages currently on queue 
  msglen_t msg_qbytes;		/* max number of bytes allowed on queue 
  __pid_t msg_lspid;		/* pid of last msgsnd() 
  __pid_t msg_lrpid;		/* pid of last msgrcv() 
  __syscall_ulong_t __glibc_reserved4;
  __syscall_ulong_t __glibc_reserved5;
};
*/


/*struct ipc_perm
{
  __key_t __key;				/* Key.  
  __uid_t uid;					/* Owner's user ID.  
  __gid_t gid;					/* Owner's group ID.  
  __uid_t cuid;					/* Creator's user ID.  
  __gid_t cgid;					/* Creator's group ID.  
  __mode_t mode;				/* Read/write permission.
  unsigned short int __seq;			/* Sequence number.  
  unsigned short int __pad2
  __syscall_ulong_t __glibc_reserved1;
  __syscall_ulong_t __glibc_reserved2;
};
*/