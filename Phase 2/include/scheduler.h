#ifndef SCH_H
#define SCH_H

#include "system.h"
#include "syscall.h"

#define FROM_INTERRUPT 0
#define FROM_SYSCALL 1

unsigned int process_time;
unsigned int kernel_time;
struct list_head ready_queue;
pcb_t *current;

void initReadyqueue();
void increaseKernelTImer();
void increaseUserTimer();

void loadProcess();
void contextSwitch(int from);

#endif