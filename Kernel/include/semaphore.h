

#ifndef TPE_SO_SEMAPHORE_H
#define TPE_SO_SEMAPHORE_H

#include <process.h>
#include <MemoryManager.h>
#include <defs.h>
#include <lib.h>
#include <scheduler.h>

#define SEMPAHORE_MANAGER_ADDRESS (0x800000)

#define MAX_SEMAPHORES 20
#define MUTEX 0
#define NOT_USED -1

typedef struct sem_t{
    uint8_t value;
    uint8_t lock;
    PCB * waiting[MAX_PROCESSES]; //queue
    uint8_t  waitingIndex;
    uint8_t  postingIndex;
    int8_t id;
} sem_t;

typedef struct semaphoreCDT * semaphoreADT;

int8_t sem_init(uint8_t semIndex, uint8_t value);
void sem_wait(uint8_t  semIndex);
void sem_post(uint8_t semIndex);
void initSemaphores();


#endif //TPE_SO_SEMAPHORE_H
