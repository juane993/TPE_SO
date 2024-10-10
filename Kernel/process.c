#include <process.h>
#include <MemoryManager.h>

uint64_t initProcess(PCB *process, void *mainFunc, uint8_t argc, char **args, uint8_t newProcessPid, Priority prio) {
    uint64_t *stackBase = allocMemory(MEM_BLOCK_SIZE);
    if (stackBase == NULL) {
        return -1;
    }

    uint64_t *rsp = initProcessStack(mainFunc, stackBase, argc, args);
    if (rsp == NULL) {
        return -1;
    }

    process->priority = prio;
    process->state = READY;
    process->processPid = newProcessPid;
    process->RSP = rsp;
    process->stackBase = stackBase;
    process->argc = argc;
    process->argv = args;

    return process->processPid;
}

uint32_t killProcess(PCB *process) {
    if (process->state == NOT_EXISTS) {
        return -1;
    }

    my_free(process->stackBase);
    process->state = NOT_EXISTS;

    return process->processPid;
}