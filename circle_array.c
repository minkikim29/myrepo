#include<stdio.h>

#define MAX_SIZE 20

int front,rear;
int queue[MAX_SIZE];

void Initq()
{
    front=rear=0;
}

int enq(int data)
{
    if((rear+1)%MAX_SIZE==front)
    {
        printf("가득참");
        return -1;
    }
    else
    {
        queue[rear]=data;
        rear=++rear%MAX_SIZE;
        return data;
    }
}

int deq()
{
    int i;
    if(front==rear)
    {
        printf("비었음");
        return -1;
    }
    else
    {
        i=queue[front];
        front=++front%MAX_SIZE;
        return i;
    }
}

void printq()
{
    int i;
    printf("\nFront------------------------------>Rear\n");
    for(i=front; i!=rear; i=++i%MAX_SIZE)
    {
        printf("%5d",queue[i]);
    }
}

int main()
{
    Initq();

    printf("enq 60, 70, 754, 899");

    enq(60);
    enq(70);
    enq(754);
    enq(899);

    printq();

    deq();
    deq();

    printq();

    printf("\n");

    return 0;
}
