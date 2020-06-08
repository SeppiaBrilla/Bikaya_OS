#ifndef SYS_H
#define SYS_H
    #include "const.h"
    #include "listx.h"
    #include "pcb.h"
    #include "types_bikaya.h"

    #ifdef TARGET_UMPS
        #include "umps/libumps.h"
        #include "umps/arch.h"
        #include "umps/types.h"
        #include "umps/cp0.h"

        //Definition of function Program Counter for uMPS
        #define PC(t) ((state_t *) t)->pc_epc
        //Definition of function Stack Pointer for uMPS
        #define SP(t) ((state_t *) t)->reg_sp
            
        #define FRAMESIZE 4*(1024*8)

        //Definition of Interrupt Lines like uARM
        #define INT_TIMER           IL_TIMER
        #define INT_DISK            IL_DISK
        #define INT_TAPE            IL_TAPE
        #define INT_UNUSED          IL_ETHERNET
        #define INT_PRINTER         IL_PRINTER
        #define INT_TERMINAL        IL_TERMINAL

        //Definition of Old and New Processor State Areas
        #define INT_OLDAREA 0X20000000
        #define INT_NEWAREA 0x2000008C
        #define TLB_OLDAREA 0x20000118
        #define TLB_NEWAREA 0x200001A4
        #define PGMTRAP_OLDAREA 0x20000230
        #define PGMTRAP_NEWAREA 0x200002BC
        #define SYSBK_OLDAREA 0x20000348
        #define SYSBK_NEWAREA 0x200003D4

        //Enabled Interrupt for uMPS
        #define INTERRUPT_ENABLE (1 << 0)
        #define INTERRUPT_ENABLE_PREVIOUS (1 << 2)
        //Enabled processor Local Timer
        #define PROCESS_LOCAL_TIMER (1 << 27)
        //Enabled external Interrupts on the Interrupt Mask 
        #define IM_INTER_PROCESSOR_INTERRUPTS (2 << 8)
        #define IM_PROCESSOR_LOCAL_TIMER (2 << 9)
        #define IM_BUS (2 << 10)
        #define IM_DISK_DEVICES (2 << 11)
        #define IM_TAPE_DEVICES (2 << 12)
        #define IM_NETTWORK_DEVICES (2 << 13)
        #define IM_PRINTER_DEVICES (2 << 14)
        #define IM_tERMINAL_DEVICES (2 << 15)

    #endif

    #ifdef TARGET_UARM
        #include "uarm/libuarm.h"
        #include "uarm/arch.h"
        #include "uarm/uARMtypes.h"
        #include "uarm/uARMconst.h"
        //Definition of function Program Counter for uARM
        #define PC(t) ((state_t *) t)->pc
        //Definition of function Stack Pointer for uARM
        #define SP(t) ((state_t *) t)->sp
 
    #endif

    //Definition of number 3 for the System Call
    #define TERMINATE_PROCESS 3
    //Definition of RAMTOP using actual Ram size and Ram base
    #define RAMBASE *((unsigned int *)BUS_REG_RAM_BASE)
    #define RAMSIZE *((unsigned int *)BUS_REG_RAM_SIZE)
    #define RAMTOP (RAMBASE + RAMSIZE)

    #define TIME_SCALE *((unsigned int *) BUS_REG_TIME_SCALE)
    #define TIME_SLICE 3000*TIME_SCALE
    
#endif
