#include "IVTEntry.h"
#include "KEvent.H"
#include "manager.h"
#include <DOS.H>

IVTEntry::IVTEntry(RoutinePointer func, IVTNo num)
{
	routines[num] = func;
}

void IVTEntry::getOld(RoutinePointer old, IVTNo num)
{
	old = events[num]->oldRoutine;
}

void IVTEntry::signal(IVTNo num)
{
	threadPut(events[num]->myThreadPCB);
}