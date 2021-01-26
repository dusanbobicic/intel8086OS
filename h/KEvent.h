//File: KEvent.h
#ifndef _KEvent_h_
#define _KEvent_h_

#include <dos.h>

#include "event.h"
#include "PCB.h"


typedef void interrupt (*RoutinePointer)(...);

RoutinePointer replaceRoutine(IVTNo ivtNo, RoutinePointer newRoutine);

class KernelEv
{
public:
	KernelEv(PCB* owner, IVTNo ivtNo);

	~KernelEv();

	void wait();

	void signal();

	void override();

	void restoreRoutine();

	IVTNo ivt;

	Queue BlockedList;
	PCB* myThreadPCB;

	RoutinePointer oldRoutine;
};
#endif