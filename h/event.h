// File: event.h 
#ifndef _event_h_ 
#define _event_h_ 
#include "IVTEntry.h"
typedef unsigned char IVTNo; 
class KernelEv; 
class Event { 
public: 
   Event (IVTNo ivtNo); 
  ~Event (); 
   void wait  (); 
protected: 
   friend class KernelEv; 
   void signal(); // can call KernelEv 
private: 
   KernelEv* myImpl; 
}; 

#define PREPAREENTRY(ivtNo, callOld)\
	KernelEv IVTEntry::*events[256];\
	void interrupt intr##ivtNo() {\
		RoutinePointer old;\
		IVTEntry::getOld(old, ivtNo);\
		IVTEntry::signal(ivtNo);\
		if (callOld) {\
			old();\
		}\
	}\
	IVTEntry Instance##ivtNo((RoutinePointer) intr##ivtNo, (IVTNo) ivtNo);

#endif 