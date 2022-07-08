#include<stdio.h>           /*for printf()*/
#include<signal.h>          /*for sigaction()*/
#include<unistd.h>          /*for pause()*/
#include<stdlib.h>

void DieWithError(char* errorMessage)
{
    perror("errorMessage");
    exit(1);
}

void InterruptSignalHandler(int signalType)
{
    printf("Interrupt Received. Exiting program\n");
    exit(1);
}

int main(int argc, char* argv[])
{
    struct sigaction handler;               /*Signal handler specification structure*/

    /*Set InterruptSignalHandler() as handler function*/
    handler.sa_handler=InterruptSignalHandler;
    /*Create mast that masks all signals*/
    if(sigfillset(&handler.sa_mask)<0)
        DieWithError("sigfillset() failed");
    /*No flags*/
    handler.sa_flags=0;

    /*Set signal handling for interrupt signals*/
    if(sigaction(SIGINT,&handler,0)<0)
        DieWithError("sigaction() failed");

    for(; ;)
        pause();                            /*suspend program until signal received*/

    exit(0);
}
