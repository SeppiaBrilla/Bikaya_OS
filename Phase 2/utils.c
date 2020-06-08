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
   newArea->status = STATUS_TE;
#endif
#ifdef TARGET_UARM
   //mask interrupt, kernel mode and vm of
   newArea->cpsr = STATUS_ALL_INT_DISABLE(newArea->cpsr);
   newArea->CP15_Control = CP15_DISABLE_VM(newArea->CP15_Control);
#endif
}

void initProcess(state_t *proc, void *programCounter, unsigned int stackPointer)
{
   //setting pc and sp for standard function
   PC(proc) = programCounter;
   SP(proc) = stackPointer;
#ifdef TARGET_UMPS
   //interrupt and timer enable
   proc->status = ALL_INT_ENABLE;
#endif
#ifdef TARGET_UARM
   //interrupt enable and disable vm
   proc->cpsr = STATUS_ALL_INT_ENABLE((proc->cpsr) | STATUS_SYS_MODE);
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

void initSem()
{ //initialize the devices' semaphores
    for (int i = 0; i < N_EXT_IL + 1; i++)
        for (int j = 0; j < N_DEV_PER_IL; j++)
            SemDevices[i][j] = 0;
}

void saveState(state_t *newPcb, state_t *ps)
{ //copy the ps state in newpcb
#ifdef TARGET_UMPS
    unsigned int i;
    newPcb->entry_hi = ps->entry_hi;
    newPcb->cause = ps->cause;
    newPcb->status = ps->status;
    newPcb->pc_epc = ps->pc_epc;
    newPcb->hi = ps->hi;
    newPcb->lo = ps->lo;
    for (i = 0; i < 29; i++)
        newPcb->gpr[i] = ps->gpr[i];
#endif
#ifdef TARGET_UARM
    newPcb->a1 = ps->a1;
    newPcb->a2 = ps->a2;
    newPcb->a3 = ps->a3;
    newPcb->a4 = ps->a4;
    newPcb->v1 = ps->v1;
    newPcb->v2 = ps->v2;
    newPcb->v3 = ps->v3;
    newPcb->v4 = ps->v4;
    newPcb->v5 = ps->v5;
    newPcb->v6 = ps->v6;
    newPcb->sl = ps->sl;
    newPcb->fp = ps->fp;
    newPcb->ip = ps->ip;
    newPcb->sp = ps->sp;
    newPcb->lr = ps->lr;
    newPcb->pc = ps->pc;
    newPcb->cpsr = ps->cpsr;
    newPcb->CP15_Control = ps->CP15_Control;
    newPcb->CP15_EntryHi = ps->CP15_EntryHi;
    newPcb->CP15_Cause = ps->CP15_Cause;
    newPcb->TOD_Hi = ps->TOD_Hi;
    newPcb->TOD_Low = ps->TOD_Low;
#endif
}