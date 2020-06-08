#ifndef SYSCLL_H
    #define SYSCLL_H

    #include "system.h"
    #include "scheduler.h"

    #define TERMINAL_ADDRESS_LOW (DEV_REG_START + (N_EXT_IL - 1) * N_DEV_PER_IL * DEV_REG_SIZE)

    #ifdef TARGET_UARM
    #define sys_return ((state_t *)SYSBK_OLDAREA)->a1
    #define ID ((state_t *)SYSBK_OLDAREA)->a1
    #define arg1 ((state_t *)SYSBK_OLDAREA)->a2
    #define arg2 ((state_t *)SYSBK_OLDAREA)->a3
    #define arg3 ((state_t *)SYSBK_OLDAREA)->a4
    #endif
    #ifdef TARGET_UMPS
    #define sys_return ((state_t *)SYSBK_OLDAREA)->reg_v0
    #define ID ((state_t *)SYSBK_OLDAREA)->reg_a0
    #define arg1 ((state_t *)SYSBK_OLDAREA)->reg_a1
    #define arg2 ((state_t *)SYSBK_OLDAREA)->reg_a2
    #define arg3 ((state_t *)SYSBK_OLDAREA)->reg_a3
    #endif

    int softBlockCount;
    void handlerSYS();
    int Terminate_Process(pcb_t* current);

#endif