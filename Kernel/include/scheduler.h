#ifndef _SCHEDULER_H
#define _SCHEDULER_H

#include <stdint.h>
#define SCHEDULER_ADDRESS (0x700000)
#define MAX_PROCESSES 70

typedef enum {
    READY,
    BLOCK,
    RUNNING,
    NOT_EXISTS
} State;



typedef struct schedulerCDT  *schedulerADT;
#endif