#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct Node{
    struct Node* next;
    int data;
}node;

typedef struct Queue{
    node* front;
    node* rear;
    int count;
}queue;

void Initq(queue* q)
{
    q->front=q->rear=NULL;
    q->count=0;
}

int IsEmpty(queue* q)
{
    return q->count==0;
}

void enq(queue* q,int data)
{
    node* now=(node*)malloc(sizeof(node));
    now->data=data;
    now->next=NULL;

    if(IsEmpty(q))
    {
        q->front=now;
    }
    else
    {
        q->rear->next=now;
    }
    q->rear=now;
    q->count++;
}

int deq(queue* q)
{
    int re=0;
    node* now;
    if(IsEmpty(q))
    {
        return re;
    }
    else
    {
        now=q->front;
        re=now->data;
        q->front=now->next;
        free(now);
        q->count--;
    }
}

void printq(queue* q)
{
    node* curr=q->front;
    printf("\nFront---------------------------------Rear\n");
    while(curr!=NULL)
    {
        printf("%5d",curr->data);
        curr=curr->next;
    }
}

int main()
{
    queue q;
    Initq(&q);
    
    printf("\nEnq 20, 90, 260, 50, 110\n");
    enq(&q,20);
    enq(&q,90);
    enq(&q,260);
    enq(&q,50);
    enq(&q,110);
    printq(&q);

    printf("\nDeq 두번\n");
    deq(&q);
    deq(&q);

    printq(&q);

    printf("\n");

    return 0;
}
