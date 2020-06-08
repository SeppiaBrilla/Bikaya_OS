#include "scheduler.h"

void initReadyqueue() { INIT_LIST_HEAD(&ready_queue); }

void increaseUserTimer() 
{
    kernel_time = getTODLO();
    if (current)
        current->user_time += kernel_time - process_time;
}

void increaseKernelTImer()
{ 
    if (current) 
        current->kernel_time += getTODLO() - kernel_time; 
}

void set_start_kernel_time() { kernel_time = getTODLO(); }

void set_start_user_time() { process_time = getTODLO(); }

HIDDEN void aging()
{ // running through the ready queue and increasing priority of all process
    struct list_head *p;

    list_for_each(p, &ready_queue)
    {
        struct pcb_t *pcb = container_of(p, pcb_t, p_next);
        pcb->priority++;
    }
}

void loadProcess()
{

    //removing the process with highest priority and making it current
    current = removeProcQ(&ready_queue);

    //increasing remaing processes priority
    aging();

    //setting timer equal to TIME_SLICE (3ms)W
    setTIMER(TIME_SLICE);

    if (current != NULL)
    {
        //loading the process into the cpu
        set_start_user_time();
        LDST(&(current->p_s));
    }
    else
    {
        //if there's no process in the ready queue but there's some process waiting for device, the OS wait for the interrupt from one of the devices
        if (softBlockCount > 0)
        {
            setSTATUS(STATUS_ALL_INT_ENABLE(getSTATUS()));
            WAIT();
        }
        else
        {
            PANIC();
        }
    }
}

HIDDEN void saveProcess(int from)
{
    if (current != NULL)
    {
        //updating current process status
        switch (from)
        {
        case FROM_INTERRUPT: //different memory area to copy from, based on the handler which called the context switch
            memcpy(&(current->p_s), INT_OLDAREA, sizeof(state_t));
            break;
        case FROM_SYSCALL:
            increaseKernelTImer();
            memcpy(&(current->p_s), SYSBK_OLDAREA, sizeof(state_t));
            break;
        default:
            PANIC();
            break;
        }

        //resetting original priority and inserting into readyQueue
        current->priority = current->original_priority;
        insertProcQ(&ready_queue, current);
    }
    current = NULL;
}

void contextSwitch(int from)
{
    saveProcess(from);
    loadProcess();
}
