//File: timer.cpp
#include <iostream.h>
#include <dos.h>
#include <stdlib.h>

#include "timer.h"
#include "PCB.h"
#include "manager.h"


extern void tick();

volatile int lockFlag=1;
volatile int counter=20;
volatile int demanded_context_switch=0;
volatile int sleepTime;

volatile unsigned tsp;
volatile unsigned tss;
volatile unsigned tbp;


static volatile PCB* tmp,*curr,*prev;
extern volatile Queue *Blocked;

void interrupt timer()		// prekidna rutina /na svakih 55ms?
{
	counter--;
	if ((counter == 0 && lockFlag) || demanded_context_switch)
	{
		#ifndef BCC_BLOCK_IGNORE
		asm {
			// cuva sp, ss i bp
			mov tsp, sp
			mov tss, ss
			mov tbp, bp
		}
		#endif
		PCB::running->sp = tsp;
		PCB::running->ss = tss;
		PCB::running->bp = tbp;

		switch (demanded_context_switch)
		{
		case 1:	// dispatch
			if (PCB::running->done == 0)
				threadPut((PCB*)PCB::running);
			break;
		case 2:	// block
			curr = PCB::running;
			Blocked->put((PCB*)curr);
			break;
		case 3:	// shutdown
			curr = PCB::running;
			PCB::Finished.put((PCB*)curr);
			break;
		case 4:	// sleep
			curr = PCB::running;
			PCB::Sleeping.put((PCB*)curr);
			break;
		case 5:	// yield
			threadPut((PCB*)PCB::running);
			PCB::running = tmp;
			break;
		default:
			if (PCB::running->done == 0)
				{threadPut((PCB*)PCB::running);}
			break;
		}
		PCB::running = threadGet();

		tsp = PCB::running->sp;
		tss = PCB::running->ss;
		tbp = PCB::running->bp;

		counter = PCB::running->time;
		#ifndef BCC_BLOCK_IGNORE
		asm {
			mov sp, tsp   // restore sp
			mov ss, tss
			mov bp, tbp
		}
		#endif
    }
	if(PCB::Sleeping.head)
	{
		curr = PCB::Sleeping.head->next;
		prev = PCB::Sleeping.head;
		while (curr)
		{
			if (curr->timeToSleep)
				curr->timeToSleep--;
			if (!curr->timeToSleep)
			{
				tmp = curr;
				prev->next = curr->next;
				if(prev->next==0){PCB::Sleeping.tail=prev;}
				tmp->next = 0;
				threadPut((PCB*)tmp);
				curr = curr->next;
			}
			else
			{
				prev = prev->next;
				curr = curr->next;
			}
		}
		if ((PCB::Sleeping.head)->timeToSleep)
		{
			(PCB::Sleeping.head)->timeToSleep--;
		}
		if (!(PCB::Sleeping.head)->timeToSleep)
		{
			tmp = PCB::Sleeping.head;
			PCB::Sleeping.head = PCB::Sleeping.head->next;
			tmp->next = 0;
			threadPut((PCB*)tmp);
		}
	}
    if(!demanded_context_switch) 
	{
		tick();
		#ifndef BCC_BLOCK_IGNORE
		asm int 60h;
		#endif
	} //pozivamo staru prekidnu
    else {
		demanded_context_switch=0;
	}
}
unsigned oldTimerOFF, oldTimerSEG; // stara prekidna rutina

// postavlja novu prekidnu rutinu
void inic()
{
	#ifndef BCC_BLOCK_IGNORE
	asm {
		cli
		push es
		push ax

		mov ax,0   //  ; inicijalizuje rutinu za tajmer
		mov es,ax

		mov ax, word ptr es:0022h //; pamti staru rutinu
		mov word ptr oldTimerSEG, ax
		mov ax, word ptr es:0020h
		mov word ptr oldTimerOFF, ax

		mov word ptr es:0022h, seg timer	 //postavlja
		mov word ptr es:0020h, offset timer //novu rutinu

		mov ax, oldTimerSEG	 //	postavlja staru rutinu
		mov word ptr es:0182h, ax //; na int 60h
		mov ax, oldTimerOFF
		mov word ptr es:0180h, ax

		pop ax
		pop es
		sti
	}
	#endif
}

// vraca staru prekidnu rutinu
void restore()
{
	#ifndef BCC_BLOCK_IGNORE
	asm {
		cli
		push es
		push ax

		mov ax,0
		mov es,ax


		mov ax, word ptr oldTimerSEG
		mov word ptr es:0022h, ax
		mov ax, word ptr oldTimerOFF
		mov word ptr es:0020h, ax

		pop ax
		pop es
		sti
	}
	#endif
}
