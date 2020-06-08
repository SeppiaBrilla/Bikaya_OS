#include "syscall.h"

#ifdef TARGET_UARM
#define ID ((state_t *)SYSBK_OLDAREA)->a1
#endif
#ifdef TARGET_UMPS
#define ID ((state_t *)SYSBK_OLDAREA)->reg_a0
#endif

void handlerSYS()
{
    //UMPS need pc decrease by 4 
    #ifdef TARGET_UMPS
        PC(SYSBK_OLDAREA) += 4;
    #endif

    //Recognizing systemcall type and handling it, only terminate process for phase 1.5
    switch (ID)
    {
        case TERMINATE_PROCESS:
            Terminate_Process(current);
            break;

       /* case 4:
            break;

        case 5:
            break;

        case 6:
            break;

        case 7:
            break;

        case 8:
            break;

        case 9:
            break;

        case 10:
            break;

        case 11:
            break;*/
        default :
            PANIC();
            break;
    }
    loadProcess();
}

void handlerTRAP()
{
    ;
}

void Terminate_Process(pcb_t *p)
{
    //kill the current process and all his child by recursive call
    struct list_head *tmp;
    if (!emptyChild(p))
    {
        list_for_each(tmp, &(p->p_sib)){
            struct pcb_t *pcb = container_of(p, pcb_t, p_next);
            Terminate_Process(pcb);
        }
    }
    freePcb(p);
}
