//File: system.cpp
#include <dos.h>

#include "system.h"

extern volatile int demanded_context_switch;
extern volatile int lockFlag;
void sys_dispatch()
{
	#ifndef BCC_BLOCK_IGNORE
	asm cli;
	demanded_context_switch = 1;
	asm	int 8h;
	asm sti;
	#endif
}

void sys_block()
{
	demanded_context_switch = 2;
	#ifndef BCC_BLOCK_IGNORE
	asm	int 8h;
	#endif
}

void sys_shutdown()
{
	demanded_context_switch = 3;
	#ifndef BCC_BLOCK_IGNORE
	asm	int 8h;
	#endif
}

void sys_sleep()
{
	#ifndef BCC_BLOCK_IGNORE
	asm pushf;
	asm cli;
	demanded_context_switch = 4;
	asm popf;
	#endif
	#ifndef BCC_BLOCK_IGNORE
	asm	int 8h;
	#endif
}

void sys_yield()
{
	demanded_context_switch = 5;
	#ifndef BCC_BLOCK_IGNORE
	asm	int 8h;
	#endif
}

void sys_stop()
{
	demanded_context_switch = 6;
	#ifndef BCC_BLOCK_IGNORE
	asm	int 8h;
	#endif
}

void softLock()
{
	#ifndef BCC_BLOCK_IGNORE
	asm pushf;
	asm cli;
	lockFlag = 0;
	asm popf;
	#endif
}

void softUnlock()
{
	#ifndef BCC_BLOCK_IGNORE
	asm pushf;
	asm cli;
	lockFlag = 1;
	asm popf;
	#endif
}
