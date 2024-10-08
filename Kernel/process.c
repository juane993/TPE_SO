//
// Created by juan on 06/10/24.
//
#include <process.h>

void endlessLoop()
{
    while(1);
}

void (*endlessLoopPtr)() = endlessLoop;

uint64_t initProcess(PCB * process ,void * mainFunc,uint8_t argc, char ** args, uint8_t currentRunningProcesses)
{

    uint64_t *stackBase = allocMemory(MEM_BLOCK_SIZE);
    if(stackBase == NULL){
        return -1;
    }
    uint64_t * rsp = initProcessStack(mainFunc,stackBase,argc,args);
    if(rsp == NULL){
        return -1;
    }
    process->state = READY;
    process->processPid = currentRunningProcesses;
    process->RSP = rsp;
    process->stackBase = stackBase;
    process->argc = argc;
    process->argv = args;
    return process->processPid;
}