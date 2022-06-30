#include<stdio.h>
#include<stdlib.h>

typedef struct Node{
    struct Node* next;
    struct Node* prev;
    int data;
}node;

node* head;
node* tail;

void addhead(int data)
{
    node* new=(node*)malloc(sizeof(node));
    new->data=data;
    new->next=head->next;
    head->next=new;
    new->prev=new->next->prev;
    new->next->prev=new;
}

void addtail(int data)
{
    node* new=(node*)malloc(sizeof(node));
    new->data=data;
    new->prev=tail->prev;
    tail->prev=new;
    new->next=new->prev->next;
    new->prev->next=new;
}

void delhead()
{
    node* new=head->next;
    head->next=new->next;
    node* next=new->next;
    next->prev=head;

    free(new);
}

void deltail()
{
    node* new=tail->prev;
    tail->prev=new->prev;

    node* next=new->prev;
    next->next=tail;
    free(new);
}

void printhead()
{
    node* curr=head->next;
    while(curr!=tail)
    {
        printf("%4d",curr->data);
        curr=curr->next;
    }
    printf("\n");
}

void printtail()
{
    node* curr=tail->prev;
    while(curr!=head)
    {
        printf("%4d",curr->data);
        curr=curr->prev;
    }
    printf("\n");
}

int main()
{
    head=(node*)malloc(sizeof(node));
    tail=(node*)malloc(sizeof(node));
    head->next=tail;
    head->prev=NULL;
    tail->prev=head;
    tail->next=NULL;

    addhead(20);
    addhead(50);
    addhead(30);
    addhead(40);
    addhead(80);
    addhead(90);
    printhead();
    printtail();

    delhead();
    deltail();

    printhead();
    printtail();

    return 0;
}
