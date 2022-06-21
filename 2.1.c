2.1 "생성(Creating)과 해지(Destroying)

int socket(int protocolFamily,int protocol) "첫번째 파라미터는 프로토콜 패밀리 결정, 두번째는 소켓의 형태 지정

int close(int socket) "애플리케이션이 소켓을 종료할 때, close()를 호출.

2.2 "주소 지정
struct sockaddr
{
    unsigned short sa_family; /*Address family (e.g. AF_INET)*/ "주소가 속하는 공간을 정의
    char sa_data[14];         /*Family-specific address information*/ "주소 패밀리에 따라 정해지는 비트들의 조합
};


struct in_addr "구조체 sockaddr의 특정 형태로 TCP/IP 소켓 주소를 위해 사용하는 구조체
{
    unsigned long s_addr; /*Internet address(32 bits)*/
};

struct sockaddr_in "주소 패밀리에 추가하여 포트 번호와 인터넷 주소를 위한 필드를 가지고 있다.
{
    unsigned short sin_family; /*Internet protocol (AF_INET)*/
    unsigned short sin_port;   /*Address port(16bits)*/
    struct in_addr sin_addr;   /*Internet address (32bits)*/
    char sin_zero[8];          /*Not used*/
};

int connect(int socket, struct sockaddr* foreignAddress, unsigned int addressLength)
    socket "socket()에 의해 생성된 식별자
    foreignAddress "sockaddr_in을 가리키는 포인터
    addressLength "주소 구조체의 길이

int send(int socket,const void* msg,unsigned int msgLength,int flags)
int revc(int socket,void* rcvBuffer,unsigned int bufferLength,int flags)
    msg "보낼때의 메시지, msgLength는 이 메시지의 길이

