#include<stdio.h>
#include<stdlib.h>

typedef struct Node{
    struct Node* next;
    int data;
}node;

void Initnode(node* head)
{
    head->next=NULL;
}

void addnode(node* head,int data)
{
    node* now=(node*)malloc(sizeof(node));
    now->data=data;
    now->next=head->next;
    head->next=now;
}

int delnode(node* head)
{
    node* tar=head->next;
    head->next=tar->next;
    free(tar);
}


void printnode(node* head)
{
    node* curr;
    printf("\n-------------------------Node-------------------------\n");
    for(curr=head->next; curr!=NULL; curr=curr->next)
    {
        printf("%5d",curr->data);
    }
}

int main()
{
    node* head=(node*)malloc(sizeof(node));
    head->next=NULL;
    Initnode(head);

    addnode(head,20);
    addnode(head,10);
    addnode(head,900);
    addnode(head,60);
    addnode(head,80);

    printnode(head);

    delnode(head);
    delnode(head);

    printnode(head);

    printf("\n");

    return 0;
}
