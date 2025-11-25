//fork 와 exec 함수를 이해하기 위하 예제
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void child_start();

int main(int argc, char **argv){
    pid_t pid;  
    int child_status, child_return;
    
    puts("\t parent process start.......");
    if((pid=fork())<0){
        perror("fork failed : ");
        exit(0);
    }

    else if (pid==0){
        child_start();
    }
    else if (pid>0){
        printf("\n\t**parent : [my pid: %d] my child pid=%d\n",getpid(),pid);

    }        
    return 0;
}

void child_start(){
    puts("\t child process start...");
    printf("\t**child : [my pid: %d] my parent pid=%d\n",getpid(),getppid());
    printf("\n\t**exec()함수로 ls 명령을 수행합니다\n");
    execlp("ls","ls",NULL);  // l : list 형태의 인자를 취함, p : 명령을 PATH에서 검색하겠다는 의미

    perror("exec error at child: ");
    exit(0);
}

//흐름
// 1. fork() 전까지는 부모만 실행
// 2. fork() 이후 부모와 자식이 동시에 실행
// 3. 자식은 exec() 호출 -> ls 프로그램으로 변신
// 4. 부모는 자식 PID 출력 후 종료
// 5. 출력 순서는 항상 일정하지 않음(동시 실행 때문)