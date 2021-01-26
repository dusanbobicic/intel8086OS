//File: KSem.cpp
#include "KSem.h"
#include "PCB.h"
#include "Queue.h"
#include "system.h"
#include "manager.h"

extern volatile Queue *Blocked;
 KernelSem::KernelSem(Semaphore *sem,int value){
     mySem=sem;
     val=value;
 }
 int KernelSem::wait(int toBlock){
     if(toBlock==0){
         
         if(val<=0)return -1;
         val--;
         return 0;
     }
     else{
         val--;
         if(val<0)
         {
             Blocked=&BlockList;
             sys_block();
             return 1;
         }
         else
         {
             return 0;
         }
     }

 }
 int KernelSem::signal(){
     if((val<0)&& BlockList.head)
     {
         volatile PCB* item=BlockList.getFirst();
         /*if(item!=0)*/threadPut((PCB*)item);
     }
     val++;
     return 1;
 }
 KernelSem::~KernelSem(){
     
 }
int KernelSem::value()const{
     return val;
 }
