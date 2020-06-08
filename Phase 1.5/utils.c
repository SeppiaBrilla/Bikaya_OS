#include "utils.h"

void initArea(unsigned int area, unsigned int handler)
{
   state_t *newArea = (state_t *)area;
   STST(newArea);
   SP(newArea) = RAMTOP;
   // pc set at handler entry point
   PC(newArea) = handler;

#ifdef TARGET_UMPS
   //mask interrupt, kernel mode and vm of 
   newArea->status = PROCESS_LOCAL_TIMER;
#endif
#ifdef TARGET_UARM
   //mask interrupt, kernel mode and vm of 
   newArea->cpsr = STATUS_ALL_INT_DISABLE(newArea->cpsr);
   newArea->CP15_Control = (newArea->CP15_Control);
#endif
}

void initProcess(state_t *proc, void *programCounter, unsigned int stackPointer)
{
   //setting pc and sp for standard function
   PC(proc) = programCounter;
   SP(proc) = stackPointer;
#ifdef TARGET_UMPS
   //interrupt and timer enable 
   proc->status = INTERRUPT_ENABLE + INTERRUPT_ENABLE_PREVIOUS + PROCESS_LOCAL_TIMER + IM_PROCESSOR_LOCAL_TIMER; 
#endif
#ifdef TARGET_UARM
   //mask interrupt but timer and disable vm 
   proc->cpsr = (proc->cpsr | STATUS_SYS_MODE);
   proc->cpsr = STATUS_DISABLE_INT(proc->cpsr);
   proc->cpsr = STATUS_ENABLE_TIMER(proc->cpsr);
   proc->CP15_Control = CP15_DISABLE_VM(proc->CP15_Control);
#endif
}

void memcpy(void *dest, void *src, size_tt n)
{
   // Typecast src and dest addresses to (char *)
   char *csrc = (char *)src;
   char *cdest = (char *)dest;

   // Copy contents of src[] to dest[]
   for (int i = 0; i < n; i++)
      cdest[i] = csrc[i];
}