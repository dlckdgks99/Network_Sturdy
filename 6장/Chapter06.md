# 6.1 데몬 프로세스
- 데몬 : 백그라운드로 실행되는 프로세스로서 특정 터미널의 제어와 관계없이 실행되는 프로세스를 말함.
- 데몬을 종료시키려면 kill 명령을 사용하거나 다른 프로세스에서 SIGKILL 시그널을 데몬으로 보내야 함
- TTY(터미널 장치)를 가지고 있지 않음
- PPID(parent id)가 1이고, SID(session id) 자신의 아이디와 같음
  
### 데몬 프로세스 생성
- 데몬 프로세스 생성 방법
  1. fork()를 호출하여 자식 프로세스를 만들고 부모 프로세스는 종료
  2. setsid()를 호출하여 자신을 새로운 프로세스 그룹의 리도로 만듬 =>자기를 생성한 부모의 터미널 PID와 관계를 끊기 위해
  3. 시그널 SOGHUP를 무시하겠다는 sigaction(()함수를 호출하고 자식 프로세스를 한 번더 만든 후 부모 프로세스 종료
  4. 두 번째 자식 프로세스는 이제 어떤 터미널과도 연관이 없다
  5. 작업 디렉토리를 루트 디렉토리로 변경하고 mask를 0으로 설정하여 새로 생성되는 파일이 임의의 소유 권한을 가질 수 있도록 함
  6. 혹시 부모 프로세스가 개설하였을지 모르는 소켓을 모두 닫기 위해 close() 실행
- setid()
  - 현재 프로세스를 새로운 세션(Session)의 리더로 만들고, 터미널과 완전히 분리하는 함수
### 데몬 서버 종류
- 서버 프로그램을 독립형 데몬으로 구현(Standalone)
  - httpd, sendmail, named
  - 항상 실행되고 있으면서 서비스를 제공해 주는 데몬
  - 특정 서비스를 위한 전용 프로그램이기에 빠르게 응답하나 항상 실행되어있기에 비효율적
- 슈퍼 데몬 (Inetd)
  - 여러 가지 인터넷 서비스 요청을 받아서 처리해 주는 종합겆인 인터넷 서비스용 데몬
  - echo, daytime같은 간단한 인터넷 서비스 요청은 Inetd에서 직접 처리
  - telnet, ftp와 같은 서비스 요청은 inetd가 그 서비스를 처리하기 위한 새로운 프로세스를 생성
  - 각 서비스를 구분하기 위해 포트번호 사용
 
### Inetd 환경 설정 (Internet Service Daemon)
-  $ /etc/services
-  $ /etc/inetd.conf 

### TCP Wrapper
- 외부에서 telnet, ftp 등의 TCP/IP 응용 서비스를 이용하려고 할 때 서비스 제공 여부를 상대방 IP주소를 보고 제한하는 기능 제공
- TCP Wrapper 기능은 tcpd 데몬이 제공
- $/etc/hosts.deny
- $/etc/hosts.allw

# xinetd를 이용한 서버 구축
### xinetd 환경 설정
- $ /etc/xinetd.conf : xinetd가 지원하는 인터넷 서비스와 해당 포트 등을 정의
- $ /etc/rc.d/init.d/xinetd : defaults 부분과 service 부분으로 나누어짐 
  - defaults 부분 설정
    - instances : 하나의 서비스에 대해 동시에 처리할 수 있는 최대 요청의수
    - long_type : 프로그램에 의해 생성되는 로그의 형태를 어떤 식으로 나타낼 지
    - log_on_success : 서버가 서비스를 시작할 때 어떤 정보가 로그에 남을 것인지를 정의
    - only_from : 서비스를 이용할 수 있도록 허용하는 원격 호스트들을정의
    - per_source : 특정 서비스별로 접속할 수 있는 최대 접속자 수를 설정
    - enabled : xinetd에서 지원하는 서비스 이름을 설정
  - service 부분 설정
    - socket_type : 이 서비스가 사용할 소켓의 타입을 지정
    - wait : 
    - user : 
    - type : 서비스의 유형을 정의
    - id : 
    - protocol : 
    - only_from : 접근을 허용할 원격 주소 정의
    - no_access : 접근을 차단할 원격 호스트 주소를 정의

### xinetd를 이용한 에코 서버
- 소켓 관련 함수 호출이 전혀없음
- 그 이유는 에코 서비스를 위해 지정된 포트번호로 서비스 요청이 들어오면 xinetd가 accept()를 대신 호출하여 클라이언트와 연결하고 echoserv.c를 실행시켜주기에
- xinetd가 클라이언트와 연결된 후 소켓으로의 입력 및 출력을 서버 응용 프로그램의 표준 입력 및 출력과 내부적으로 연결시켜줌
- 응용프로그램에서 stdout으로 출력된 데이터는 클라이언트로 전송되며, 클라이언트로부터 수신된 데이터는 stdin으로 읽음
- xinetd에 서비스를 등록하는 절차
  - /etc/service 파일에 myecho_serv 서버를 등록
  - /etc/xinetd.d/myecho_serv라는 서비스 파일을 만들어 xinetd와 관련된 환경을 설정
  - xinetd 재시작
    
### UDP 에코 서버

# 🤔6장에 대한 내 생각
- sigaction 구조체 오류
  - #include <bits/sigaction.h>라이브러리에 있었음!!
- 포트 열려있는지 확인 명령어
  - sudo lsof -i :9901
- 요즘에는 inetd를 안쓰기에 설치해줘야한다 : $sudo apt install openbsd-inetd
- 증권사와 데몬 서버 구축이 어떤관계를 가지고 있을까?
  - xinetd
    - 보안 정책 때문에 xinetd 기반 서비스는 대부분 금지  
    - 요청이 올 때마다 프로세스를 띄우는 모델은 너무 느리고 비효율적이라 사용하지 않는다고함
