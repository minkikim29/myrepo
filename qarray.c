#include<stdio.h>
#include<stdlib.h>

#define MAX_SIZE 10

int front=-1,rear=-1;
int queue[MAX_SIZE];

int Initq()
{
    rear=-1;
    front=-1;
}

int IsFull()
{
    if(rear==MAX_SIZE-1)
        return 1;
    else
        return 0;
}

int IsEmpty()
{
    if(front==rear)
        return 1;
    else
        return 0;
}

void enqueue(int data)
{
    if(IsFull())
    {
        printf("가득참");
    }
    queue[rear++]=data;
}

int dequeue()
{
    if(IsEmpty())
    {
        printf("비어있음");
    }
    return queue[front++];
}

int main(void)
{
    int data,i;
    Initq();

    for(i=1; i<10; i++)
    {
        if(!IsFull())
        {
            enqueue(i);
        }
    }

    while(!IsEmpty())
    {
        printf("%d",dequeue());
    }

    printf("\n");
    return 0;
}
    

