# 5.1 소켓 옵션 변경
### 소켓 옵션 변경 함수
- getsockopt() : extern int getsockopt(int fd, int level, int optname, void *optval, socklen_t *optlen);  
  - fd: 옵션을 조회할 소켓 번호
  - level: 옵션 레벨 (SOL_SOCKET, IPPROTO_IP, IPPROTO_TCP 등)
  - optname: 조회할 옵션 이름
  - optval: 옵션 값을 저장할 버퍼(포인터)
  - optlen: optval의 크기를 나타내는 변수(입력 + 출력)

- setsockopt() : extern int setsockopt(int fd, int level, int optname, const void *optval, socklen_t optlen);
  - fd: 옵션을 설정할 소켓 번호
  - level: 옵션 레벨 (SOL_SOCKET, IPPROTO_IP, IPPROTO_TCP 등)
  - optname: 설정할 옵션 이름
  - optval: 설정할 옵션 값이 저장된 버퍼(포인터)
  - optlen: optval의 크기(바이트 단위)




# 번외
### gcc VS cc 차이
| 명령 | 의미                       | 실제 컴파일러         | 특징                               |
| ---- | -------------------------- | ---------------------- | ---------------------------------- |
| gcc  | GNU C 컴파일러 직접 호출  | GCC                    | 강력한 경고/최적화, 가장 많이 사용 |
| cc   | 시스템 기본 C 컴파일러 호출 | GCC 또는 Clang 등     | OS에 따라 달라서 환경 의존적       |










# 🤔 5장에 대한 나의 생각
- 도메인 주소 변환함수[gethostbyname(), gethostbyaddr()] 와 소켓 옵션 변경함수 [getsockopt(), setsockopt()] 헷갈렸었음
