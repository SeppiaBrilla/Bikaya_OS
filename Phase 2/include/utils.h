#ifndef UTILS_H
#define UTILS_H

#include "system.h"

int SemDevices[N_EXT_IL + 1][N_DEV_PER_IL];

void initArea(unsigned int area, unsigned int handler);
void initProcess(state_t *proc, void* programCounter, unsigned int stackPointer);
void memcpy(void *source, void *dest, size_tt size);
void initSem();
void saveState(state_t *newPcb, state_t *ps);
#endif

