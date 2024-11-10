

#ifndef TPE_SO_MEMORYMANAGER_H
#define TPE_SO_MEMORYMANAGER_H

#include <stdint.h>
#include <lib.h>

#define NULL ((void *)0)


#define START_FREE_MEM 0x600000


//buddy system
#define TOTAL_MEM_SIZE 0x20000  //128kb = 131072 bytes


#define START_AUX_STRUCT (void *)0x620000 //for buddy control system. UNUSED
#define sizeof_buddy_node 32
#define MIN_BLOCK_SIZE 0x400 //1kb



#define MAX_HEIGHT 7



//not buddy system
#define BLOCK_COUNT 200

void createMemoryManager(int size);
void* allocMemory(uint32_t size);
void my_free(void *p);

#endif //TPE_SO_MEMORYMANAGER_H
