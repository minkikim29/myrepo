#include<stdio.h>
#include<stdlib.h>

#define MAX_SIZE 30

int stack[MAX_SIZE];
int top=-1;

int IsFull()
{
    if(top==MAX_SIZE-1)
    {
        printf("에러 스택이 가득참");
        return 1;
    }
    return 0;
}

int IsEmpty()
{
    if(top==-1)
    {
       return 1;
    }
    return 0;
}

void Push(int data)
{
    if(!IsFull())
    {
        top++;
        stack[top]=data;
    }
}

int Pop()
{
    if(!IsEmpty())
    {
        return stack[top--];
    }
}

int main()
{
    int i;
    for(i=1; i<=20; i++)
    {
        if(!IsFull())
        {
            Push(i);
        }
    }
    

    while(!IsEmpty())
    {
       printf(" %d",Pop());
    }
    printf("\n");
    return 0;
    
}
