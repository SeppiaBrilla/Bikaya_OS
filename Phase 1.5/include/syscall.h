#ifndef SYSCLL_H
#define SYSCLL_H

#include "system.h"
#include "utils.h"
#include "scheduler.h"


#ifdef TARGET_UMPS
    #define ID ((state_t *)SYSBK_OLDAREA)->reg_a0
#endif

#ifdef TARGET_UARM
    #define ID ((state_t *)SYSBK_OLDAREA)->a1
#endif

void handlerSYS();
void handlerTRAP();
void Terminate_Process(pcb_t* current);

#endif