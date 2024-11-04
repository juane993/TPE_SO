#include <process.h>
#include <MemoryManager.h>
#include <scheduler.h>
#include <semaphore.h>

// OJO, quiero ver si tengo una race condition y por eso explota
//despues debe haber mejor manera de hacerlo pero quiero que no me interrumpan
//cuando estoy cerrando la wrapper
extern void disableInterrupts();
extern void enableInterrupts();



uint64_t initProcess(PCB *process, void *mainFunc, uint8_t argc, char **args, uint8_t newProcessPid, Priority prio, BackgroundState bgState, uint8_t parentPid) {
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
    process->parentPid = parentPid;
    for (int i = 0; i < 40; ++i) {
        process->waitingForMe[i] = NULL;
    }

    //process->argv = args;
    process->bgState = bgState;
    process->waitingIndex = 0;
    //process->waitingForMe[0] = getProcess(parentPid);

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
    sem_wait(MUTEX);
    unblockWaiters(getCurrentPid());
    zombie(getCurrentPid());
    sem_post(MUTEX);
    yield();
}