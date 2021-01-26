//File: PCB.h
#ifndef _PCB_h_
#define _PCB_h_

#include "thread.h"
#include "Queue.h"

class Queue;
class PCB{
    public: 
    volatile unsigned sp;
    volatile unsigned ss;
    volatile unsigned bp;
    volatile unsigned time;
    volatile unsigned done;
    volatile Time timeToSleep;
    volatile ID UID;

    volatile PCB* next;
    volatile PCB* children;
    volatile static PCB* running;
    volatile static Queue Sleeping,Finished;
    volatile Queue *BlockedList;

    Thread* myThread;

    PCB();
    PCB(Thread* th,Time time, StackSize stackSize);
    PCB(Time time);
    PCB(Time time, void (*body)());

    ID getID();
    void start();
    void exitThread()volatile;
    void waitToComplete() volatile;

    static void wrapper(); 
    static void sleep(Time tts);

    void createProcess(void (*body)(), unsigned long stackSize );
};
void stop();
#endif