#include "system.h"
#include "syscall.h"
#include "scheduler.h"
#include "interrupt.h"

extern void test();

int main()
{
    //Populating New Areas
    initArea(INT_NEWAREA, handlerINT);
    initArea(SYSBK_NEWAREA, handlerSYS);
    initArea(PGMTRAP_NEWAREA, handlerTRAP);
    initArea(TLB_NEWAREA, handlerTLB);

    initReadyqueue();

    //Pcb and variables initialization
    initPcbs();

    initASL();

    initSem();
    
    pcb_t *process;
    softBlockCount = 0;

    //Process initialization and queue insertion
    process = allocPcb();
    initProcess(&(process->p_s), &test, RAMTOP - FRAMESIZE);
    process->priority = 1;
    process->original_priority = 1;
    process->origin_time = getTODLO();

    insertProcQ(&ready_queue, process);

    //The scheduler is started
    loadProcess();

    return 0;
}