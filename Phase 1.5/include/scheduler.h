#ifndef SCH_H
#define SCH_H

#include "pcb.h"
#include "system.h"
#include "utils.h"

struct list_head ready_queue;
pcb_t *current;

void initReadyqueue();
void aging();
void loadProcess();
void saveProcess();
void contextSwitch();
void useless();

#endif