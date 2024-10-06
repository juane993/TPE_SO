//
// Created by juan on 06/10/24.
//
#include <process.h>



uint64_t * initProcess(void * mainFunc,uint8_t argc, char ** args)
{

    void * stackBase = allocMemory(MEM_BLOCK_SIZE);
    uint64 * rsp = initProcessStack(mainFunc,stackBase,argc,args);
    return rsp;

    //pushFlags, entryPoint, bp
    //pushstate
}