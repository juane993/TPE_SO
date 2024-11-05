#ifndef TPE_SO_PROCESS_H
#define TPE_SO_PROCESS_H

#include <stdint.h>
#include <MemoryManager.h>
#include <lib.h>
#include <defs.h>

typedef struct  WaitingProcess {
    struct PCB * waitingPCB;
    uint8_t boolUnblock;
} WaitingProcess;

typedef struct PCB {
    uint8_t priority;
    uint8_t processPid;
    uint8_t parentPid;
    State state;
    BackgroundState bgState;
    uint8_t *RSP;
    uint8_t *stackBase;
    char **argv;
    uint8_t argc;
    WaitingProcess waitingForMe[40];
    uint8_t waitingIndex;
    struct PCB * children[40];
    uint8_t childIndex;
} PCB;




void processWrapper(char argc, char *argv[],void * mainFunc);
uint64_t initProcess(PCB *process, void *mainFunc, uint8_t argc, char **args, uint8_t newProcessPid, Priority prio, BackgroundState bgState, uint8_t parentPid);
uint32_t killProcess(PCB *process);

#endif // TPE_SO_PROCESS_H