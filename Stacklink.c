#include<stdio.h>
#include<stdlib.h>

typedef struct Node{
    struct Node* next;
    int data;
}node;


void Push(node* head,int data)
{
    node* now=(node*)malloc(sizeof(node));
    now->data=data;
    now->next=head->next;
    head->next=now;
}

int Pop(node* head)
{
    node* tar=head->next;
    head->next=tar->next;
    free(tar);
}

void printstack(node* head)
{
    node* curr=head->next;
    printf("\nTop-------------------------------Bottom\n");
    while(curr!=NULL)
    {
        printf("%5d",curr->data);
        curr=curr->next;
    }
}

int main()
{
    node* head=(node*)malloc(sizeof(node));
    head->next=NULL;

    printf("\nPush 30, 40, 9090, 80, 50\n");
    Push(head,30);
    Push(head,40);
    Push(head,9090);
    Push(head,80);
    Push(head,50);
    printstack(head);

    Pop(head);
    Pop(head);

    printstack(head);

    printf("\n");

    return 0;
}
