//
// Created by Nep on 9/20/2024.
//

#ifndef TPE_SO_MEMORYMANAGER_H
#define TPE_SO_MEMORYMANAGER_H

#include <stdint.h>
#include <lib.h>

#define NULL ((void *)0)

#define BLOCK_COUNT 20
#define START_FREE_MEM 0x600000

void createMemoryManager(int size);
void* allocMemory(uint32_t size);
void my_free(void *p);

#endif //TPE_SO_MEMORYMANAGER_H
