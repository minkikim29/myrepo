#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#define RCVBUFSIZE 32
#define MAXPENDING 5

void DieWithError(char* errorMessage)
{
    perror(errorMessage);
    exit(1);
}

void HandleTCPClient(int clntSocket)
{
    char echoBuffer[RCVBUFSIZE];
    int recvMSgSize;

    if((recvMSgSize=recv(clntSocket, echoBuffer, RCVBUFSIZE, 0))<0)
        DieWithError("recv() failed");

    while(recvMSgSize>0)
    {
        if(send(clntSocket, echoBuffer, recvMSgSize, 0)!=recvMSgSize)
            DieWithError("send() failed");

        if((recvMSgSize=recv(clntSocket, echoBuffer, RCVBUFSIZE, 0))<0)
            DieWithError("recv() failed");
    }

    close(clntSocket);
}

int CreateTCPServerSocket(unsigned short port)
{
    int sock;
    struct sockaddr_in echoServAddr;

    /*Create socket for incoming connections*/
    if((sock=socket(PF_INET,SOCK_STREAM,IPPROTO_TCP))<0)
        DieWithError("socket() failed");

    /*Construct local address structure*/
    memset(&echoServAddr, 0, sizeof(echoServAddr));
    echoServAddr.sin_family=AF_INET;
    echoServAddr.sin_addr.s_addr=htonl(INADDR_ANY);
    echoServAddr.sin_port=htons(port);

    /*Bind to the local address*/
    if(bind(sock,(struct sockaddr*)&echoServAddr,sizeof(echoServAddr))<0)
        DieWithError("bind() failed");

    /*Mark the socket so it will listen for incoming connections*/
    if(listen(sock, MAXPENDING)<0)
        DieWithError("listen() failed");

    return sock;
}

int AcceptTCPConnection(int servSock)
{
    int clntSock;
    struct sockaddr_in echoClntAddr;
    unsigned int clntLen;

    /*Set the size of the in-out parameter*/
    clntLen=sizeof(echoClntAddr);

    /*Wait for a client to connect*/
    if((clntSock=accept(servSock,(struct sockaddr*)&echoClntAddr,
                    &clntLen))<0)
        DieWithError("accept() failed");

    /*clntSock is conneted to a client!*/

    printf("Handling client %s\n", inet_ntoa(echoClntAddr.sin_addr));

    return clntSock;
}

