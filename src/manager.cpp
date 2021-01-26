#include "manager.h"
#include "schedule.h"
#include "PCB.h"

PCB* initial = new PCB();

extern volatile unsigned runningThreads;

void idle()
{
	while (runningThreads);
}

PCB idlePCB(1, idle);

void threadPut(PCB* thread)
{
	asm pushf;
	asm cli;
	if (thread != &idlePCB)
		Scheduler::put(thread);
	asm popf;
}

PCB* threadGet()
{
	PCB* selected = Scheduler::get();
	if (selected)
		return selected;
	else if (runningThreads)
		return &idlePCB;
	else
		return initial;
}
