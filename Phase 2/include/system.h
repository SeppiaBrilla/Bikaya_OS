#ifndef SYS_H
    #define SYS_H
    #include "const.h"
    #include "listx.h"
    #include "pcb.h"
    #include "asl.h"
    #include "types_bikaya.h"
    #include "const_bikaya.h"
    #include "utils.h"

    #ifdef TARGET_UMPS
        #include "umps/libumps.h"
        #include "umps/arch.h"
        #include "umps/types.h"
        #include "umps/cp0.h"

        //Definition of function Program Counter for uMPS
        #define PC(t) ((state_t *)t)->pc_epc
        //Definition of function Stack Pointer for uMPS
        #define SP(t) ((state_t *)t)->reg_sp

        //#define BUS_TODLOW 0x1000001c
        #define getTODLO() (*((unsigned int *) BUS_REG_TOD_LO))

        //Definition of Old and New Processor State Areas
        #define INT_OLDAREA 0X20000000
        #define INT_NEWAREA 0x2000008C
        #define TLB_OLDAREA 0x20000118
        #define TLB_NEWAREA 0x200001A4
        #define PGMTRAP_OLDAREA 0x20000230
        #define PGMTRAP_NEWAREA 0x200002BC
        #define SYSBK_OLDAREA 0x20000348
        #define SYSBK_NEWAREA 0x200003D4

        //Enabled external Interrupts on the Interrupt Mask
        #define ALL_INT_ENABLE ((STATUS_IEp) | (STATUS_IM_MASK) | (STATUS_TE))

        #define STATUS_ALL_INT_ENABLE(status) ((status)  | (STATUS_IEc) | (STATUS_IEp) | (STATUS_IM_MASK) | (STATUS_TE))

        /*Fase 2*/
        #define ACK 1

        #define return_reg(t) ((state_t *)t)->reg_v0

        #define DEV_CHARRECV 5
        #define DEV_CHARTRSM 5

    #endif

    #ifdef TARGET_UARM
        #include "uarm/libuarm.h"
        #include "uarm/arch.h"
        #include "uarm/uARMtypes.h"
        #include "uarm/uARMconst.h"

        //Definition of function Program Counter for uARM
        #define PC(t) ((state_t *)t)->pc
        //Definition of function Stack Pointer for uARM
        #define SP(t) ((state_t *)t)->sp

        /*Fase 2*/
        #define ACK DEV_C_ACK

        #define return_reg(t) ((state_t *)t)->a1

        #define DEV_CHARRECV DEV_TRCV_S_CHARRECV
        #define DEV_CHARTRSM DEV_TTRS_S_CHARTRSM

    #endif

    //Definition of RAMTOP using actual Ram size and Ram base
    #define RAMBASE *((unsigned int *)BUS_REG_RAM_BASE)
    #define RAMSIZE *((unsigned int *)BUS_REG_RAM_SIZE)
    #define RAMTOP (RAMBASE + RAMSIZE)

    #define TIME_SCALE *((unsigned int *)BUS_REG_TIME_SCALE)
    #define TIME_SLICE 3000 * TIME_SCALE

    #define TERMSTATMASK 0xFF

#endif
