# 10. Raw 소켓
- Raw 소켓을 이용하면 임의의 프로토콜 헤더를 만들어 전송할 수 있으면 IO 헤더는 커널에 의해 자동으로 만드ㅡㄹ어진 것을 사용할 수 있음
# 10.1 Raw 소켓 생성
- s=socket(AF_INET,SOCK_RAW,protocol);
- Raw 소켓을 총하여 데이터그램을 전송하려면 sendto(), sendmsg()를 사용해야하고, 데이터그램을 수신하려면 recvfrom(),recvmsg() 함수를 사용해야함

# 10.2 PING 프로그램 구현
### 프로그램 개요
- 사용자 IP 헤더 작성
- ICMP 에코 요청 전송
- ICMP EHCO 응답 수신
- 
