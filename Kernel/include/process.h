#ifndef TPE_SO_PROCESS_H
#define TPE_SO_PROCESS_H

#include <stdint.h>
#include <MemoryManager.h>
#include <lib.h>
#include <defs.h>

typedef enum {
    READY,
    BLOCK,
    RUNNING,
    NOT_EXISTS
} State;



typedef struct PCB {
    uint8_t priority;
    uint8_t processPid;
    //uint8_t parentPid;
    State state;
    uint8_t * RSP;
    uint8_t * stackBase;
    char * argv;
    uint8_t argc;
} PCB;


uint64_t initProcess(PCB * process, void * mainFunc,uint8_t argc, char ** args, uint8_t currentRunningProcesses);

#endif //TPE_SO_PROCESS_H
