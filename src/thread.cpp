#include <dos.h>
#include <IOSTREAM.H>
#include "thread.h"
#include "PCB.h"
#include "system.h"

Thread::~Thread()
{
    asm pushf;
    asm cli;
    delete myPCB;
    asm popf;

}

Thread::Thread(StackSize stackSize, Time timeSlice)
{
    myPCB=new PCB(this,timeSlice,stackSize);
}

void Thread::start()
{
    myPCB->start();
}

ID Thread::getID()
{
    if(myPCB)return myPCB->getID();
}

void Thread::waitToComplete()
{
    myPCB->waitToComplete();

} 

void Thread::sleep(Time timeToSleep){
    PCB::sleep(timeToSleep);
}

void dispatch()
{
	sys_dispatch();
}