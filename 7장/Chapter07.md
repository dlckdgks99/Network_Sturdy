# 7.1 시그널 처리
- 시그널 : 프로세스에서 어떤 이벤트가 발생한 것을 다른 프로세스에게 알리는 도구

### 시그널의 종류
- SIGINT : 인터럽트 시그널, 사용자가 인터럽트키를 입력했을 때 더미널에서 실행시킨 프로세스에게 커널이 전달됨, 이 시그널을 수신한 프로세스는 일반적으로 바로 종료
- SIGIO : 비동기 입출력이 발생했을 때 전달
- SIGPIPE : 닫혀진 파이프에 데이터를 쓰거나 읽기를 시도할 때 발생
- SIGCHLD : 프로세스가 종료되거나 취소될 때 자신의 부모 프로세스에게 보내짐
- SIGURG : 대역외 데이터를 수신했을 때 발생
- SIGUSR1, SIGUSR2 : 사용자의 임의의 목적으로 사용할 수 있는 예비된 시그널
- 시그널 발생
  - 다른 프로세스에게 시그널이 발생되도록하려면 kill() 시스템콜 사용
  - int kill(pid_t pid, int sig);
  - int raise(int sig);

### 시그널 처리 기본 동작
- 정해진 기본 동작 수행 : 프로세스 종료 또는 시그널 무시
- 사용자가 지정한 작업 수행 : 시그널 핸들러 수행, 시그널 무시, 시그널 블록

### 시그널 핸들러
- 시그널 집합 : 시그널 처리와 각종 함수에서는 여러 개의 시그널을 한 번에 표시하기 위해서 시그널 지합 타입 siset_t를 사용, 여러 개의 시그널을 bitmask 형태로 누적하여 표현
  - sigemptyset() : 시그널 집합을 비어 있는 상태로 초기화
  - sigfillset() : 시스템에 정의되어 있는 모든 시그널들을 시그널 집합에 넣는 함수
  - sigaddset() : 시그널 집합에 특정한 시그널을 추가
  - sigdelset() : 시그널 집합에서 특정 시그널을 제거
  - sigsmember() : 시그널 집합 안에 특정 시그널이 존재하는지 여부를 확인하는 함수
    
- 시그널 핸들러 설정 : sigaction() 함수를 사용
  - int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact); = int sigaction(처리할 대상이 되는 시그널, 시그널에 대한 동작을 수정, 시그널에 대한 동작을 조회)
  - struct sigaction
    -  void(*sa_handler)(int): 시그널 핸들러를 가르킴
    -  void(*sa_sigaction)(int, siginfo_t *, void *) : 시그널 핸들러를 가리킴, 요즘에는 안쓰고 부가적인 정보만 이용할 때씀
    -  sigset_t sa_mask : 시그널 핸들러가 동작되는 동안 블록시킬 시그널 지;ㅂ합
    -  int sa_flags : 각종 환경절정을 위해 사용
      - SA_RESTART : 
      - SA_NOCLDWAIT : 좀비 프로세스를 만들지 않도록 하기위한 플래그
      - SA_NODEFER : 도착한 시그널의 수만큼 핸들러가 수행
      - SA_SIGINFO : sa_sigaction에 명시된 핸들러가 호출
      - SA_ONESHOT or SA_RESETHAND : 시그널 핸들러가 한 번 실행된 후에는 시그널 처리 기본 동작으로 되돌아가도록 함

- Signal-unsafe 함수 : 시그널 핸들러 내에서 호출하면 그 결과가 불확실한 함수
  - 시그널 핸들러 내에서 signal-unsafe 함수를 직접 호출하지 말고 핸들러 내에서는 특정 플래그만 설정해두고 핸들러가 종료된 후에 플래그를 확인하고 signal-unsafe한 함수를 핸들러 밖에서 호출하는 방식\
- signal()함수

# 7.2 시그널 처리 예
### SIGINT 처리 예
- 
