#ifndef _SCHEDULER_H
#define _SCHEDULER_H

#include <stdint.h>

typedef enum {
    READY,
    BLOCK,
    RUNNING,
    NOT_EXISTS
} State;

typedef struct PCB {
    uint8_t priority;
    uint8_t processPid;
    State state;
    uint8_t * RSP;
} PCB;

#endif