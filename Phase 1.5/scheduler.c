#include "scheduler.h"

void initReadyqueue(){
    INIT_LIST_HEAD(&ready_queue);
}

void aging(){
    struct list_head *p;

    list_for_each(p, &ready_queue)
    {
        struct pcb_t *pcb = container_of(p, pcb_t, p_next);
        pcb->priority++;
    }
}

void loadProcess()
{
    //remove the process with hightest priority and make it current
    pcb_t *procNext = removeProcQ(&ready_queue);
    current = procNext;

    //increase remaing processes priority
    aging();

    //setting timer equal to TIME_SLICE (3ms)
#ifdef TARGET_UARM
    setTIMER(TIME_SLICE * 15);
#endif
#ifdef TARGET_UMPS
   *(unsigned int*)BUS_REG_TIMER = TIME_SLICE * 15;
#endif

    if (current != NULL){
        //loading the process into the cpu
        LDST(&(current->p_s));
    }
    else{
        //There is no Ready Process left
        useless();
    }
}

void saveProcess()
{
    if (current != NULL)
    {
        //updating current process status
        memcpy(&(current->p_s), INT_OLDAREA, sizeof(state_t));

        //reset original priority and insert into readyQueue
        current->priority = current->original_priority;
        insertProcQ(&ready_queue, current);
    }
    current = NULL;
}

void contextSwitch(){
    saveProcess();
    loadProcess();
}


void useless(){
    while (1){
        ;
    }
}
