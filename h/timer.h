//File: timer.h
#ifndef _Timer_h_
#define _Timer_h_

#ifndef BCC_BLOCK_IGNORE
void interrupt timer();
#endif

void inic();

void restore();

void softLock();

void softUnlock();

#endif
