#include "KEvent.h"
#include <dos.h>
#include "system.h"
#include "PCB.h"
#include "Queue.h"
#include "manager.h"
extern volatile Queue *Blocked;
KernelEv* IVTEntry::events[256];
RoutinePointer IVTEntry::routines[256];

RoutinePointer replaceRoutine(IVTNo ivtNo, RoutinePointer newRoutine)
{
	#ifndef BCC_BLOCK_IGNORE
	asm pushf;
	asm cli;
	unsigned int newSEG = FP_SEG(newRoutine);
	unsigned int newOFF = FP_OFF(newRoutine);
	unsigned int oldSEG, oldOFF;
	#endif
	RoutinePointer old;
	ivtNo *= 4;
	#ifndef BCC_BLOCK_IGNORE
	asm {
		push es
		push ax
		push bx

		mov ax, 0
		mov es, ax
		mov bx, word ptr ivtNo

		mov ax, word ptr es:bx+2
		mov word ptr oldSEG, ax
		mov ax, word ptr es:bx
		mov word ptr oldOFF, ax

		mov ax, word ptr newSEG
		mov word ptr es:bx+2, ax
		mov ax, word ptr newOFF
		mov word ptr es:bx, ax

		pop bx
		pop ax
		pop es
	}
	old = (RoutinePointer)MK_FP(oldSEG, oldOFF);
	asm popf;
	#endif
	return old;
}

KernelEv::KernelEv(PCB* owner, IVTNo ivtNo)
{
	#ifndef BCC_BLOCK_IGNORE
	asm pushf;
	asm cli;
	#endif
	myThreadPCB = owner;
	ivt = ivtNo;
	IVTEntry::events[ivt] = this;
	oldRoutine = (RoutinePointer)replaceRoutine(ivt, (RoutinePointer)IVTEntry::routines[ivt]);
	#ifndef BCC_BLOCK_IGNORE
	asm popf;
	#endif
}

KernelEv::~KernelEv()
{
	#ifndef BCC_BLOCK_IGNORE
	asm pushf;
	asm cli;
	#endif
	IVTEntry::events[ivt] = 0;
	replaceRoutine(ivt, (RoutinePointer)oldRoutine);
	#ifndef BCC_BLOCK_IGNORE
	asm popf;
	#endif
}

void KernelEv::wait()
{
	#ifndef BCC_BLOCK_IGNORE
	asm cli;
	#endif
	if (PCB::running == myThreadPCB)
	{
		Blocked=&BlockedList;
		sys_block();
	}
	#ifndef BCC_BLOCK_IGNORE
	asm sti;
	#endif
}

void KernelEv::signal()
{
	if(BlockedList.head!=0)
	{
		BlockedList.getFirst();
	}
	threadPut(myThreadPCB);

}