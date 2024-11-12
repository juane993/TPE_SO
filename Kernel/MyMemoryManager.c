#include <MemoryManager.h>
#include <string.h>
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
uint64_t getMemUsed()
{
    return current * block_size;
}

char * printMem()
{
    char * buffer        = (char *) allocMemory(1000);
    char * currentBufPos = buffer;
    char len             = 0;
    len                  = strcpy(currentBufPos, "Using MemoryManager system\n");
    currentBufPos += len;
    len = strcpy(currentBufPos, "Total memory consumed: ");
    currentBufPos += len;
    len = itoa(getMemUsed(), currentBufPos, 16);
    currentBufPos += len;
    len = strcpy(currentBufPos, " H bytes from ");
    currentBufPos += len;
    len = itoa(TOTAL_MEM, currentBufPos, 16);
    currentBufPos += len;
    len = strcpy(currentBufPos, " H bytes\n");
    return buffer;
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
