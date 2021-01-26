//File: event.cpp
#include <iostream.h>

#include "event.h"
#include "KEvent.h"
#include "IVTEntry.h"


Event::Event(IVTNo ivtNo)
{
	myImpl = new KernelEv((PCB*)PCB::running, ivtNo);
}

Event::~Event()
{
	delete myImpl;
}

void Event::wait()
{
	myImpl->wait();
}

void Event::signal()
{
	myImpl->signal();
}
