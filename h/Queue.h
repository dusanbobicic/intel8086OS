//File: Queue.h
#ifndef _QUEUE_h_
#define _QUEUE_h_

class PCB;
class Queue
{
    public:
        Queue();
        volatile PCB* getFirst()volatile;
        void put(PCB* elem)volatile;
        void putTop(PCB* elem)volatile;
        void putList(PCB* root)volatile;
        friend class PCB;
        volatile PCB* head, *tail;
        
};
#endif