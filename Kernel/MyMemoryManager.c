#include <MemoryManager.h>

void * start;
int block_size;

void * free_ptrs[BLOCK_COUNT];
int current;


void createMemoryManager(int size)
{
    start      = (void *) START_FREE_MEM;
    block_size = size;
    for (int i = 0; i < BLOCK_COUNT; i++)
    {
        free_ptrs[i] = (char *) start + i * block_size;
    }
    current = 0;
}

void * allocMemory(uint32_t size)
{
    if (size > block_size)
    {
        return NULL;
    }
    if (current < BLOCK_COUNT)
    {
        return free_ptrs[current++];
    }
    return NULL;
}

void my_free(void * p)
{
    free_ptrs[--current] = p;
}
