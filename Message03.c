// Chapter03 메시지 구성하기

// 3.1 데이터 부호화
송신자의 메시지 구성
struct{
    int dep;
    int wd;
}msgStruct;
.
.
.
msgStruct.dep=deposits;
msgStruct.wd=withdrawals;
send(s,&msgStruct,sizeof(msgStruct),0);

TCP를 사용하고 있기 때문에 굳이 deposits와 withdrawals의 값을
구조체에 복사하지 않고 보낼 수 있다.

send(s,&deposits,sizeof(deposits),0);
send(s,&withdrawals,sizeof(withdrawals),0);


// 3.2 바이트 순서
수신측의 데이터 저장
struct{
    int deposits;
    int withdrawals;
}rcvBuf;
.
.
.
/*receive 2* sizeof(int) bytes into rcvBuf...*/

송신자를 위한 정확한 코드(4바이트의 정수)
msgStruct.dep=htonl(deposits);
msgStruct.wd=htonl(withdrawals);
send(s,&msgStruct,sizeof(msgStruct),0);


// 3.3 정렬과 채워넣기
메시지를 구성하는 구조체
struct{
    int centsDeposited;
    unsigned short numDeps;
    int centWithdrawn;
    unsigned short numWds;
}msgBuf;
.
./*assign values to fields -- convert byte order!*/
.
send(s,&msgBuf,sizeof(msgBuf),0);

정렬이 가능하도록 필드 재배치
struct MsgBuf{
    int centsDeposited;
    int centWithdrawn;
    unsigned short numDeps;
    unsigned short numWds;
}msgBuf;                    // sizeof(MsgBuf)=12


// 3.4 틀짜기(Framing) 및 파싱(Parsing)
Framing 수신자가 메시지를 파싱(메시지의 시작과 끝을 구분하고 메시지 내부
        필드들의 경계를 판별하는)할 수 있도록 정보를 포맥하는 문제.
Parsing 메시지의 시작과 끝을 구분하고 메시지 내부 필드들의 경계를 판별하는 것.

메시지의 필드들이 고정된 크기를 가지고 있을때
struct MsgBuf msg;
void* buffe=(void*)&msg;
int rBytes,rv;
.
.
.
for(rBytes=0; rBytes<sizeof(msg); rBytes+=rv)
{
    if((rv=recv(s,buffer+rBytes,
                    sizeof(msg)-rBytes,0))<=0).../*handle error*/
}
/*byte order!*/
msg.centsDeposited=ntohl(msg.centsDeposited);
.
.

파싱을 위한 코드
#define DELIMCHAR ' '
#define FIELDSPERMSG 2

int ReceiveMessage(int s, char* buf, int maxLen)
{
    int received=0;
    int delimCount=0;
    int rv;
    while((received<maxLen)&&(delimCount<FIELDSPERMSG))
    {
        rv=recv(s,buf,received,1,0);
        if(rv<0)
            DieWithError("recv() failed");
        else if(rv==0)
            DieWithError("unexpected end of transmission");
        if(*(buf+received)==DELMCHAR) /*count delimiters*/
                delimCount+=1;
        received+=1;
    }

    return received;
}
                
