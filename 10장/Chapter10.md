# 10. Raw 소켓
- Raw 소켓을 이용하면 임의의 프로토콜 헤더를 만들어 전송할 수 있으면 IO 헤더는 커널에 의해 자동으로 만드ㅡㄹ어진 것을 사용할 수 있음
# 10.1 Raw 소켓 생성
- s=socket(AF_INET,SOCK_RAW,protocol);
- Raw 소켓을 총하여 데이터그램을 전송하려면 sendto(), sendmsg()를 사용해야하고, 데이터그램을 수신하려면 recvfrom(),recvmsg() 함수를 사용해야함

# 10.2 PING 프로그램 구현
### 프로그램 개요
1. 먼저 Raw 소켓을 개설하고 ping 요청을 보낼 목적지 호스트의 IP 주소를설정함
2. Ping 요청을 보내기 위헤 ICMP헤더를 작성한 후 sendto()로 ICMP ECHO 요청을 전송 (타임 아웃 설정을 위해 alarm()호출)
3. recvfrom()로 ICMP ECHO 응답을 기다리다가 iCMP ECHO 응답이 도착하면 RTT값을 측정하여 출력
4. 만일 ICMP ECHo 응답이 도착하기 전에 타임아웃이 먼저 되면 request time out을 출력
- 사용자 IP 헤더 작성
  - ICMP란? : 네트워크의 상태 메시지(에러/진단) 담당 프로토콜
  - Ping 프로그램은 ICMP 프로토콜을 사용하므로 ICMP 헤더를 직접 구성해야함
- ICMP 에코 요청 전송
- ICMP EHCO 응답 수신
- 
