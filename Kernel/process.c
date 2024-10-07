//
// Created by juan on 06/10/24.
//
#include <process.h>



uint64_t * initProcess(void * mainFunc,uint8_t argc, char ** args, uint64_t ** stackBase)
{

    *stackBase = allocMemory(MEM_BLOCK_SIZE);
    if(*stackBase == NULL){
        return NULL;
    }
    uint64_t * rsp = initProcessStack(mainFunc,*stackBase,argc,args);
    return rsp;

    //pushFlags, entryPoint, bp
    //pushstate
}