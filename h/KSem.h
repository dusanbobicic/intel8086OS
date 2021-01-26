//File: KSem.h
#ifndef _KSEM_h_
#define _KSEM_h_

#include "Queue.h"
#include "PCB.h"
#include "semaphor.h"
#define DSV 1

class KernelSem{
    public:
    KernelSem(Semaphore *sem,int value=DSV);
    int wait(int toBlock);
    int signal();
    ~KernelSem();
    int value()const;
    private:
    Queue BlockList;
    int val;
    Semaphore *mySem;
};
#endif