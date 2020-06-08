#include "system.h"
#include "syscall.h"
#include "scheduler.h"
#include "interrupt.h"
#include "utils.h"

#include "disegno.h"
#include "grafica.h"

extern void test1();
extern void test2();
extern void test3();

void init();
void Draw();

int main()
{
    //Populate New Areas
    initArea(INT_NEWAREA, handlerINT);
    initArea(SYSBK_NEWAREA, handlerSYS);
    initArea(PGMTRAP_NEWAREA, handlerTRAP);
    initArea(TLB_NEWAREA, handlerTLB);

    initReadyqueue();
    //Pcb and variables initialization
    initPcbs();
    pcb_t *process0;
    pcb_t *process1;

    //Process initialisation and queue insertion
    process0 = allocPcb();
    initProcess(&(process0)->p_s, &init, RAMTOP - (FRAMESIZE * 1));
    process0->priority = 1;
    process0->original_priority = 1;
    insertProcQ(&ready_queue, process0);

    process1 = allocPcb();
    initProcess(&(process1)->p_s, &Draw, RAMTOP - (FRAMESIZE * 4));
    process1->priority = 2;
    process1->original_priority = 2;
    insertProcQ(&ready_queue, process1);

    //The scheduler is started
    loadProcess();

    return 0;
}