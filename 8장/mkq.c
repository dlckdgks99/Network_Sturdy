// 메시지큐를 생성하고 메시지큐 ID를 출력
//시지큐를 새로 생성하고, 생성된 메시지큐의 ID를 출력한 뒤, 다시 동일한 key로 메시지큐를 열어서 확인하는 프로그램
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
    int qid;
    key_t key;
    if(argc!=2){
        printf("Usage :%s key\n",argv[0]);
        exit(1);
    }

    key=atoi(argv[1]); //입력받은값 정수로 변환

    if((qid=msgget(key,IPC_CREAT|IPC_EXCL|0666))<0){ //이 key로 메시지큐가 없다면 새로 생성하고,이미 존재하면 오류 반환하며,생성할 때 권한은 읽기/쓰기 모두 허용
        perror("msgget fail");
        exit(EXIT_FAILURE);
    }
    printf("created queue id=%d\n",qid);

    qid=msgget(key,0);
    printf("key is = %d\n",key);
    printf("opend queue id =%d\n",qid);

    return 0;
}

/*
struct msqid_ds {
    struct ipc_perm msg_perm;  // 권한 정보
    time_t msg_stime;          // 마지막 메시지 전송 시간
    time_t msg_rtime;          // 마지막 메시지 수신 시간
    time_t msg_ctime;          // 마지막 변경 시간
    unsigned long __msg_cbytes;// 현재 큐 안 메시지 총 바이트 수
    msgqnum_t msg_qnum;        // 메시지 개수
    msglen_t msg_qbytes;       // 큐의 최대 용량
    pid_t msg_lspid;           // 마지막으로 보낸 pid
    pid_t msg_lrpid;           // 마지막으로 받은 pid
};
*/