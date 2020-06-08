#ifndef INT_H
#define INT_H

#include "scheduler.h"
#include "system.h"
#include "syscall.h"

#ifdef TARGET_UARM
    #define CAUSE_IP_GET_KAYA(val) CAUSE_IP_GET(getCAUSE(), val)
#endif
#ifdef TARGET_UMPS
    #define CAUSE_IP_GET_KAYA(val) CAUSE_IP(val) & getCAUSE()
#endif

void handlerTLB();
void handlerTRAP();
void handlerINT();

#endif