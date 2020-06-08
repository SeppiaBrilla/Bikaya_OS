#include "syscall.h"


HIDDEN void Create_Process(state_t *statep, int priority, void **cpid)
{
    pcb_t *temp = allocPcb(); //allocating a new pcb
    if (temp == NULL)
        sys_return = -1; // if it can't alloc (example: no free pcb) then return -1
    else
    {
        temp->priority = priority; //setting the pcb's fields needed
        temp->original_priority = priority;
        temp->user_time = 0;
        temp->kernel_time = 0;
        temp->origin_time = getTODLO();
        saveState(&(temp->p_s), statep);

        // inserting the new process in the queue of childs of the current process
        insertChild(current, temp);

        // adding the new process to the ready queue
        insertProcQ(&ready_queue, temp);

        if (cpid)
            *((pcb_t **)cpid) = temp;

        // return value
        sys_return = 0;
    }
}

int Terminate_Process(pcb_t *p)
{
    //killing the p process and all his children by recursive call
    if (!p)
        return 0;

    for (pcb_t *pc = removeChild(p); pc; pc = removeChild(p))
        Terminate_Process(pc);

    if (p->p_semkey != NULL)
        outBlocked(p); //removing it from the semaphore on which it's blocked
    
    outChild(p);               //removing it from his father child's list
    outProcQ(&ready_queue, p); //removing it from the readyqueue
    freePcb(p);

    return 0;
}

HIDDEN void Verhogen(int *sem)
{
    (*sem)++;                      //increasing the semaphore value
    pcb_t *p = removeBlocked(sem); //unlocking the first process locked on the queue
    if (p)
    {
        p->p_semkey = NULL;
        insertProcQ(&ready_queue, p); //putting it back on the queue (if there's one)
    }
}

HIDDEN void Passeren(int *sem)
{
    if (--(*sem) < 0)  //decreasing the semaphore value
    { //removing the process from the ready queue and locking it on the semaphore list (if needed)
        memcpy(&(current->p_s), SYSBK_OLDAREA, sizeof(state_t));
        current->p_semkey = sem;
        insertBlocked(sem, current);
        current = NULL;
    }
}

HIDDEN unsigned int WaitIO(unsigned int command, devreg_t *reg, unsigned int subdevice)
{
    //getting the chosen device's semaphore
    int index = ((memaddr)reg - DEV_REG_START) / DEV_REG_SIZE;
    int deviceType = (index / N_DEV_PER_IL);
    int device = index % N_DEV_PER_IL;

    //setting the device command field. Different handling:
    if (reg < TERMINAL_ADDRESS_LOW) //standard device
        reg->dtp.command = command;
    else
    {
        // terminal device as it can operate like 2 devices ("write" and "read")
        if (subdevice)
            reg->term.recv_command = command;
        else
            reg->term.transm_command = command;
    }
    softBlockCount++;                                        
    //increasing the waiting processes counter
    Passeren(&(SemDevices[deviceType + subdevice][device])); //blocking the device on the device's semaphore list
}

HIDDEN void SpecPassup(int type, state_t *old, state_t *new)
{
    //setting the process exception handler
    if (current->exceptions[type] == NULL && current->old_state[type] == NULL) 
    {
        current->exceptions[type] = new;
        current->old_state[type] = old;

        sys_return = 0;
    }
    else // if the execption is already setted, it kills the process
    {
        Terminate_Process(current);
        sys_return = -1;
    }
}

void handlerSYS()
{
    increaseUserTimer();

    //UMPS needs pc increased by 4
    #ifdef TARGET_UMPS
        PC(SYSBK_OLDAREA) += 4;
    #endif

    //Recognizing systemcall type and handling it
    switch (ID)
    {
        //return all process times
        case GETCPUTIME:
            *(int *)arg1 = current->user_time;
            *(int *)arg2 = current->kernel_time;
            *(int *)arg3 = getTODLO() - current->origin_time;
            break;
        
        case CREATEPROCESS:
            Create_Process((state_t *)arg1, (int *)arg2, arg3);
            break;


        //terminating the ending process, if arg1 is setted ending = pcb which id is arg1, if arg1 is NULL then ending = current process
        case TERMINATEPROCESS:;
            pcb_t *ending = arg1 ? (pcb_t *)arg1 : current;
            if(!arg1)
                current = NULL;
            sys_return = Terminate_Process(ending);
            break;

        case VERHOGEN:
            Verhogen(arg1);
            break;

        case PASSEREN:
            Passeren(arg1);
            break;

        //Wait for the execution of the current device
        case WAITIO:
            sys_return = WaitIO(arg1, arg2, arg3);
            break;

        //sets trap and tlb handlers
        case SPECPASSUP:
            SpecPassup(arg1, arg2, arg3);
            break;

        //returning the current id and the parent id on arg1 and arg2 pointed field
        case GETPID:
            if (arg1)
                *((pcb_t **)arg1) = current;
            if (arg2)
                *((pcb_t **)arg2) = current->p_parent;
            break;

        //if ID > 8 then the syscall is customed. If the syscall is setted (by specpassup) it's called, if not the process is terminated
        default:
            if (current->exceptions[0] != NULL && current->old_state[0] != NULL)
            {
                memcpy(current->old_state[0], (state_t *)SYSBK_OLDAREA, sizeof(state_t));
                LDST(current->exceptions[0]);
            }
            else
            {
                Terminate_Process(current);
                current = NULL;
            }
            break;
    }
    contextSwitch(FROM_SYSCALL);
}