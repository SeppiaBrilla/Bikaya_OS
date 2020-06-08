#include "interrupt.h"

#ifdef TARGET_UARM
    #define CAUSE_IP_GET_KAYA(val) CAUSE_IP_GET(getCAUSE(), val)
#endif
#ifdef TARGET_UMPS
    #define CAUSE_IP_GET_KAYA(val) CAUSE_IP(val)
#endif

void handlerTLB()
{
}
void handlerINT()
{   
    //UARM need pc decrease by 4 
    #ifdef TARGET_UARM
    PC(INT_OLDAREA) -= 4;
    #endif


    //interrupt recognizing and handling, only internal timer interrupt for context switch in phase 1.5    
    if (CAUSE_IP_GET_KAYA(INT_TIMER)){ //timer 
        contextSwitch();
    }
    if (CAUSE_IP_GET_KAYA(INT_DISK)){ //disk 
        ;
    }
    if (CAUSE_IP_GET_KAYA(INT_TAPE)){ //tape 
        ;
    }
    if (CAUSE_IP_GET_KAYA(INT_UNUSED)){ 
        ;
    }
    if (CAUSE_IP_GET_KAYA(INT_PRINTER)){ //printer
        ;
    }
    if (CAUSE_IP_GET_KAYA(INT_TERMINAL)){ //terminal
        ;
    }
}
