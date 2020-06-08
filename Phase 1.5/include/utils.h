#ifndef UTILS_H
#define UTILS_H

#include "system.h"

void initArea(unsigned int area, unsigned int handler);
void initProcess(state_t *proc, void* programCounter, unsigned int stackPointer);
void memcpy(void *source, void *dest, size_tt size);

#endif

