#ifndef _IVTEntry_h_
#define _IVTEntry_h_



static const int maxNumber=256;
typedef unsigned char IVTNo;
#ifndef  BCC_BLOCK_IGNORE
typedef void interrupt (*RoutinePointer)(...);
#endif
class KernelEv;

class IVTEntry
{
public:
	static KernelEv *events[256];

	static RoutinePointer routines[256];

	IVTEntry(RoutinePointer func, IVTNo num);

	static void signal(IVTNo num);

	static void getOld(RoutinePointer old, IVTNo num);
};


#endif