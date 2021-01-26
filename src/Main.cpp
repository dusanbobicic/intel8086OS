//File: Main.cpp
#include <iostream.h>
#include <stdlib.h>

#include "timer.h"
#include "thread.h"
#include "manager.h"

//#include "tstth.h"
//#include "PCB.h"

/*--EXTERNALS--*/
extern int userMain(int argc, char* argv[]);
extern volatile int runningThreads;
extern PCB* initial;
/*-------------*/
/*---GLOBALS---*/
int result;
/*-------------*/

/*----MAIN-----*/
int main(int argc, char* argv[])
{
	cout << "PROGRAM STARTEDs"<<endl;
	PCB::running = initial;
	result=-100;
	inic();
	cout << "NEW TIMER SET"<<endl;
	result = userMain(argc, argv);
	restore();
	cout << "OLD TIMER RESTORED"<<endl;
	cout<<"NUMBER OF THREADS AT END: ["<<runningThreads<<"]"<<endl;
	cout << "PROGRAM ENDED"<<endl<<"EXIT CODE: ["<<result<<"]"<< endl;
	system("pause");
	return result;
}
/*-------------*/