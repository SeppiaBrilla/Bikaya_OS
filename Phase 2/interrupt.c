#include "interrupt.h"

void handlerTLB()
{
    //TLB handler: it just calls the exception handler of the process, if there's one (setted by specpassup syscall)
    if (current->exceptions[1] != NULL && current->old_state[1] != NULL)
    {
        memcpy(current->old_state[1], (state_t *)TLB_OLDAREA, sizeof(state_t));
        LDST(current->exceptions[1]);
    }
    else
    {
    //if not, it kills the process that generated the exception
        Terminate_Process(current);
        loadProcess();
    }
}

void handlerTRAP()
{
    //TRAP handler: it just calls the exception handler of the process, if there's one (setted by specpassup syscall)
    if (current->exceptions[2] != NULL && current->old_state[2] != NULL)
    {
        memcpy(current->old_state[2], (state_t *)PGMTRAP_OLDAREA, sizeof(state_t));
        LDST(current->exceptions[2]);
    }
    else
    {
    //if not, it kills the process that generated the exception
        Terminate_Process(current);
        loadProcess();
    }
}


HIDDEN int getDevice(unsigned int line)
{ 
    //searching the device that generated the interrupt in the bitmap, the lowest are more important 
    unsigned int BitLine = 0x1;

    for (int i = 0; i < 8; i++)
    {
        if ((line & BitLine) == BitLine)
            return i;

        line = line >> 1;
    }

    return -1;
}

HIDDEN void DeviceVerhogen(int *sem, unsigned int Status_register)
{
    //removing the blocked process waiting for I/O
    pcb_t *p = removeBlocked(sem);
    (*sem)++;
    if (p)
    {
        softBlockCount--;   //updating the count of blocked process waiting for I/O 
        p->p_semkey = NULL;
        return_reg(&(p->p_s)) = Status_register; //setting the return register
        insertProcQ(&ready_queue, p); //putting the process in the ready queue 
    }
}

HIDDEN void DeviceHandler(int deviceType)
{
    //generic device handler: it gets the device, unlock the process and send an ack to the device 
    unsigned int *line = (unsigned int *)CDEV_BITMAP_ADDR(deviceType);
    int device = getDevice(*line);

    devreg_t *devreg = DEV_REG_ADDR(deviceType, device);

    DeviceVerhogen(&(SemDevices[deviceType - DEV_IL_START][device]), devreg->dtp.status);
    devreg->dtp.command = ACK;
}

HIDDEN void TerminalHandler()
{
    //same as device handler, but for terminal. It needs different treatement for "send" and "receive" character 
    unsigned int *line = (memaddr *)CDEV_BITMAP_ADDR(INT_TERMINAL);
    int device = getDevice(*line);

    devreg_t *Terminal = (devreg_t *)(DEV_REG_ADDR(INT_TERMINAL, device));
    //check if the current status of terminal is equal to the status of trasmitting character 
    if ((Terminal->term.transm_status & TERMSTATMASK) == DEV_CHARTRSM)
    {
        DeviceVerhogen(&(SemDevices[INT_TERMINAL - DEV_IL_START][device]), Terminal->term.transm_status);
        Terminal->term.transm_command = ACK;
    }
    //check if the current status of terminal is equal to the status of receiving character 
    if ((Terminal->term.recv_status & TERMSTATMASK) == DEV_CHARRECV)
    {
        DeviceVerhogen(&(SemDevices[INT_TERMINAL - DEV_IL_START + 1][device]), Terminal->term.recv_status);
        Terminal->term.recv_command = ACK;
    }
}

void handlerINT()
{
    increaseUserTimer();

    //UARM needs pc decreased by 4
    #ifdef TARGET_UARM
        PC(INT_OLDAREA) -= 4;
    #endif

    //The interrupts are recognized and handled 
    //timer
    if (CAUSE_IP_GET_KAYA(INT_TIMER))
        contextSwitch(FROM_INTERRUPT);
    //disk
    if (CAUSE_IP_GET_KAYA(INT_DISK)) 
        DeviceHandler(INT_DISK);
    //tape
    if (CAUSE_IP_GET_KAYA(INT_TAPE)) 
        DeviceHandler(INT_TAPE);
    //Ethernet (Unused)
    if (CAUSE_IP_GET_KAYA(INT_UNUSED)) {;}
    //printer
    if (CAUSE_IP_GET_KAYA(INT_PRINTER)) 
        DeviceHandler(INT_PRINTER);
    //terminal
    if (CAUSE_IP_GET_KAYA(INT_TERMINAL)) 
        TerminalHandler();
    
    contextSwitch(FROM_INTERRUPT);
}