#ifndef TPE_SO_MEMORYMANAGER_H
#define TPE_SO_MEMORYMANAGER_H

#include <stdint.h>
#include <lib.h>
#include <defs.h>

#define NULL ((void *) 0)

#define START_FREE_MEM 0x600000

#define BLOCK_COUNT 200

#define TOTAL_MEM  (BLOCK_COUNT * MEM_BLOCK_SIZE)

void createMemoryManager(int size);
void * allocMemory(uint32_t size);
void my_free(void * p);
char * printMem();

#endif // TPE_SO_MEMORYMANAGER_H
