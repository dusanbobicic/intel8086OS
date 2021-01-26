#ifndef _manager_h_
#define _manager_h_

#include "PCB.h"

void threadPut(PCB* thread);

PCB* threadGet();

#endif
