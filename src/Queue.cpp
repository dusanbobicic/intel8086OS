//File: Queue.cpp
#include "Queue.h"
#include "PCB.h"

Queue::Queue()
{
    head=tail=0;
}
volatile PCB* Queue::getFirst()volatile//vraca sa vrha reda
{
    volatile PCB* tmp;
    if(head==0)return 0;
    tmp=head;
    head=head->next;
    tmp->next=0;
    if(head==0)tail=0;
    return tmp;
}
void Queue::put(PCB* elem)volatile 
{
    
    volatile PCB* tmp;
    if(head==0)
    {head=elem;}
    else
    {
        tail->next=elem;
    }
    tail=elem;
}
void Queue::putTop(PCB* elem)volatile
{
    volatile PCB* tmp;
    tmp=head;
    head=elem;
    head->next=tmp;
}
void Queue::putList(PCB* root)volatile
{
    
    if(root==0)return;
    head=root;
    volatile PCB* tmp,*prev;
    tmp=root;
    while(tmp){prev=tmp;tmp=tmp->next;}
    if(prev!=0)tail=prev;
    else{tail=root;}
}