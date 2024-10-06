#ifndef TPE_SO_PROCESS_H
#define TPE_SO_PROCESS_H

#include <stdint.h>
#include <MemoryManager.h>
#include <lib.h>
#include <defs.h>

typedef struct processNames{
    char * name;
    void * mainFunc;
}processNames;

typedef struct PCB {
    uint8_t priority;
    uint8_t processPid;
    State state;
    uint8_t * RSP;
    uint8_t * stackBase;
    char * argv;
} PCB;


uint8_t initProcess();

#endif //TPE_SO_PROCESS_H
