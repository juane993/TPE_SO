
#ifndef TPE_SO_BUDDY_H
#define TPE_SO_BUDDY_H

#include <defs.h>
#include <stdint.h>


#define START_FREE_MEM 0x600000
#define TOTAL_MEM_SIZE 0x20000

#define START_AUX_STRUCT  (void *) 0x620000
#define sizeof_buddy_node 32
#define MIN_BLOCK_SIZE    0x400

void createMemoryManager(int size);
void * allocMemory(uint32_t size);
void my_free(void * p);
char * printMem();

#define MAX_HEIGHT 7

#endif // TPE_SO_BUDDY_H
