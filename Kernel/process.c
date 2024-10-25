#include <process.h>
#include <MemoryManager.h>
#include <scheduler.h>

uint64_t initProcess(PCB *process, void *mainFunc, uint8_t argc, char **args, uint8_t newProcessPid, Priority prio, BackgroundState bgState) {
    uint64_t *stackBase = allocMemory(MEM_BLOCK_SIZE);
    if (stackBase == NULL) {
        return -1;
    }
    process->argc = argc;
    process->argv = allocMemory(699);
    for (int i = 0; i < argc; ++i) {
        process->argv[i] = allocMemory(20);
        memcpy(process->argv[i], args[i], 20);
    }
    uint64_t *rsp = initProcessStack(processWrapper, stackBase, argc, process->argv, mainFunc);
    if (rsp == NULL) {
        for (int i = 0; i < argc; ++i) {
            my_free(process->argv[i]);
        }
        my_free(process->argv);
        my_free(stackBase);
        return -1;
    }

    process->priority = prio;
    process->state = READY;
    process->processPid = newProcessPid;
    process->RSP = rsp;
    process->stackBase = stackBase;

    //process->argv = args;
    process->bgState = bgState;

    return process->processPid;
}

uint32_t killProcess(PCB *process) {
    if (process->state == NOT_EXISTS) {
        return -1;
    }
    for (int i = 0; i < process->argc; ++i) {
        my_free(process->argv[i]);
    }
    my_free(process->argv);
    my_free(process->stackBase);
    process->state = NOT_EXISTS;
    return process->processPid;
}

void processWrapper(char argc, char *argv[], void *mainFunc) {
    uint64_t ret = ((int (*)(int, char **))mainFunc)(argc, argv);
    while (1){
        ;
    }
    killProcess(getCurrentPid());
}