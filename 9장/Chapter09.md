# 9.1 스레드 개요
- 스레드
  - 독립적으로 수행되는 프로그램 코드
  - 경량 프로세스, 프로세스 내에서 독립적으로 수행되는 제어의 흐름
  - 한 프로세스 내에서 생성된 스레드들은 서로 전역 변수를 공유하므로 스레드간에 데이터를 편리하게 공유
  - IPC기능을 사용하지 않아도 된다
  
### 스레드 생성과 종료
- pthread_create()
- pthread_self()
- pthread_exit()
- pthread_join()
- Posix 스레드 라이브러리
- 스레드의 생성과 종료 예
- REENTRANT
  - #define_REENTRANT를 선언한 효과를 줌
  - 일반 함수들이 멀티스레드 환경에 적합하게 동작하도록 해줌
  
### 스레드의 취소
- 스레드의 취소 : 스레드에서 다른 스레드의 실행을 종료시키는 것
  - 취소요청 수용
    - cancellation point에서 취소요청을 받으면 항상 즉시 종료 : read(),write(), open(), close(), fcntl(), sleep(),wait(),waitpid(),thread_join()
    - 즉시 취소 : cancellation point가 아니어도 즉시 종료
    - 지연 취소 : cancellation point에 도달할 때까지 기다린 후 종료
  - 취소요청 무시
- 스레드의 취소 관련 함수
  - int phread_setcancelstate(int state, int *oldstate) : 취소요청의 수용여부를 설정하는 함수
    - PTHREAD_CANCEL_ENABLE : 취소요청을 받아들인다.
    - PTHREAD_CANCEL_DISABLE : 취소요청을 무시한다.
  - int phread_setcanceltype(int type, int *oldtype)
    - PTHREAD_CANCEL_ASYNCHRONOUS : 즉시취소
    - PTHREAD_CANCEL_DEFERRED : cancellation point를 만날 때까지 연기
  - void phread_testcancel() : 현재 도착한 취소요청이 있는 지를 검사하고 도착한 취소요청이 있으면 스레드를 종료시키는 함수
- 취소요청을 무시하는 예
- 지연 취소
- 즉시 취소
### 스레드의 상태
- 준비
- 실행
- 블록
- 종료
### 스레드의 동기화문제
- 스레드 경쟁 예
- 플래그 사용 예
# 9.2 뮤텍스
### 뮤텍스 사용 방법
- 뮤텍스 사용 절차
- 뮤텍스 선언 및 초기화
- 기본 뮤텍스
- Timed 타입 뮤텍스
- Recursive 타입 뮤텍스
- Error Check 타입 뮤텍스
- 뮤텍스의 삭제
- 뮤텍스와 데드락

## 뮤텍스 사용 예

### 스레드의 Cleanup 핸들러
- cleanup 핸들러 등록
# 9.3 스레드의 조건변수
### 조건변수 사용 방법
- 조건변수의 정의
- 조건변수의 동작
- 조건변수의 생성과 삭제
### 조건변수 사용 예
### 스레드의 세마포어

# 9.4 스레드의 시그널 처리
### 스레드 시그널 처리함수

# 9.5 멀티스레드 에코 서버

#  🤔 9장에 대한 내 생각
- 컴파일 lpthread, -D_REENTRANT 쓰는이유
  - lpthread : pthread 라이브러리를 링크해서 pthread_create, pthread_join 같은 심볼을 해결
  - D_REENTRANT : glibc를 멀티스레드 안전 모드로 컴파일하게 하여 errno, stdio, malloc 등을 스레드별(TLS)로 분리

