//File: PCB.cpp
#include <iostream.h>
#include <dos.h>

#include "schedule.h"
#include "Queue.h"
#include "PCB.h"
#include "thread.h"
#include "manager.h"
#include "timer.h"
#include "system.h"

class Queue;
volatile PCB* PCB::running=0;
volatile Queue *Blocked=0;
volatile Queue PCB::Sleeping,PCB::Finished;
volatile PCB *tmp=0,*prev=0;
volatile unsigned runningThreads;
volatile ID id=0;
PCB::PCB()
{
    #ifndef BCC_BLOCK_IGNORE
    asm pushf;
    asm cli;
    #endif
    UID=++id;
    time=defaultTimeSlice;
    next=0;
    children = 0;
    myThread=0;
    done=0;
    BlockedList=new Queue();
    #ifndef BCC_BLOCK_IGNORE
    asm popf;
    #endif
}
PCB::PCB(Thread* th,Time time, StackSize stackSize)
{
    #ifndef BCC_BLOCK_IGNORE
    asm pushf;
    asm cli;
    #endif
    UID=++id;
    this->time=time;
    next=0;
    children = NULL;
    myThread=th;
    createProcess(wrapper,stackSize);
    BlockedList=new Queue();
    done=0;
    #ifndef BCC_BLOCK_IGNORE
    asm popf;
    #endif

}
PCB::PCB(Time time, void (*body)())
{
    #ifndef BCC_BLOCK_IGNORE
	asm pushf;
	asm cli;
    #endif
    UID=++id;
	this->time = time;
	next = 0;
    children = NULL;
	createProcess(body, defaultStackSize);
    BlockedList=new Queue();
	done = 0;
    #ifndef BCC_BLOCK_IGNORE
	asm popf;
    #endif
}
PCB::PCB(Time time)
{
    #ifndef BCC_BLOCK_IGNORE
	asm pushf;
	asm cli;
    #endif
    UID=++id;
	this->time = time;
	children = NULL;
	next = 0;
	done = 0;
    BlockedList=new Queue();
    #ifndef BCC_BLOCK_IGNORE
	asm popf;
    #endif
}


void PCB::createProcess(void (*body)(), StackSize stackSize )//new context
{
    stackSize/=sizeof(unsigned); //iz bajtova u broj elemenata
    unsigned *stack= new unsigned[stackSize];

    #ifndef BCC_BLOCK_IGNORE 
    stack[stackSize-1]=FP_SEG(myThread);
    stack[stackSize-2]=FP_OFF(myThread);

    //-3 -4 callback fun

    stack[stackSize - 5] = 0x200;	//PSW I flag
    stack[stackSize - 6] = FP_SEG(body);
    stack[stackSize - 7] = FP_OFF(body);
    // -6 -> -15 registers

    sp=FP_OFF(stack+stackSize-16);
    ss=FP_SEG(stack+stackSize-16);
    bp=FP_OFF(stack+stackSize-16);

    #endif
}

ID PCB::getID(){return this->UID;}

void PCB::start()
{
    #ifndef BCC_BLOCK_IGNORE
    asm pushf;
    asm cli;
    #endif
    runningThreads++;
    done=0;
    threadPut(this);
    #ifndef BCC_BLOCK_IGNORE
    asm popf;
    #endif
}
void PCB::waitToComplete()volatile
{
    #ifndef BCC_BLOCK_IGNORE
    asm cli;
    #endif
    if(!done)
    {
        tmp=children;
        if(tmp){
            this->BlockedList->putList((PCB*)tmp);
        }
        Blocked=this->BlockedList;
        sys_block();
    }
    #ifndef BCC_BLOCK_IGNORE
    asm sti;
    #endif
}

void PCB::exitThread()volatile
{
    #ifndef BCC_BLOCK_IGNORE
    asm cli;
    #endif
    tmp=(PCB*)children;
	while (children)
	{
		tmp=children;
		threadPut((PCB*)tmp);
		children=children->next;
		tmp->next=0;
	} 

	done = 1;
	sys_shutdown();

    #ifndef BCC_BLOCK_IGNORE
	asm sti;
    #endif
}
void PCB::sleep(Time tts)
{
    #ifndef BCC_BLOCK_IGNORE
    asm cli;
    #endif
    PCB::running->timeToSleep=tts;
    sys_sleep();
    #ifndef BCC_BLOCK_IGNORE
    asm sti;
    #endif
}
void PCB::wrapper()
{
	PCB::running->myThread->run();
	runningThreads--;
	PCB::running->exitThread();
}
void stop()
{
    #ifndef BCC_BLOCK_IGNORE
	asm cli;
    #endif
	sys_stop();
    #ifndef BCC_BLOCK_IGNORE
	asm sti;
    #endif
}
