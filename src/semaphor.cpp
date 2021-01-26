#include "semaphor.h"
#include "KSem.h"
#include <dos.h>

Semaphore::Semaphore(int init){
    asm pushf;
	asm cli;
    myImpl=new KernelSem(this,init);
    asm popf
    } 
  Semaphore::~Semaphore(){
    asm pushf;
	asm cli;
	delete myImpl;
	asm popf;
  }
  int Semaphore::wait (int toBlock){
    asm pushf;
	asm cli;
	int res = myImpl->wait(toBlock);
	asm popf;
	return res;
  }
  void Semaphore::signal(){
  asm pushf;
	asm cli;
	myImpl->signal();
	asm popf;
  }
  int Semaphore::val () const{return myImpl->value(); }
