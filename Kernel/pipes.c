#include <pipes.h>

typedef struct pipesCDT{
    pipe_t pipes[MAX_PIPES];
} pipesCDT;

pipesADT pipesManager;

void initPipeManager()
{
    pipesManager = (pipesADT) PIPES_MANAGER_ADDRESS;
    for (uint8_t i = 0; i < MAX_PIPES; i++) {
        pipesManager->pipes[i].pipeId = NOT_USED;
    }
}

int8_t initPipe(uint8_t pipeId){
    if(pipeId >= MAX_PIPES ||pipeId < 0 || pipesManager->pipes[pipeId].pipeId != NOT_USED)
    {
        return -1;
    }
    pipesManager->pipes[pipeId].pipeId = pipeId;
    pipesManager->pipes[pipeId].buffer[0] = 0;
    pipesManager->pipes[pipeId].readIndex = 0;
    pipesManager->pipes[pipeId].writeIndex = 0;
    return pipeId;
}

int8_t closePipe(uint8_t pipeId){
    if(pipeId >= MAX_PIPES ||pipeId < 0 || pipesManager->pipes[pipeId].pipeId == NOT_USED)
    {
        return -1;
    }
    pipesManager->pipes[pipeId].pipeId = NOT_USED;
    return pipeId;
}

int8_t writePipe(uint8_t pipeId, char * buf, uint8_t size){
    if(pipeId >= MAX_PIPES ||pipeId < 0 || pipesManager->pipes[pipeId].pipeId == NOT_USED)
    {
        return -1;
    }
    pipe_t * pipe = &pipesManager->pipes[pipeId];
    if((size + pipe->writeIndex) >= PIPE_BUF_SIZE)
    {
        size = PIPE_BUF_SIZE - (pipe->writeIndex);
    }
    memcpy(pipe->buffer + (pipe->writeIndex), buf, size);
    pipe->writeIndex += size;
    return size;
}

int8_t readPipe(uint8_t pipeId, char * buf, uint8_t size){
    if(pipeId >= MAX_PIPES ||pipeId < 0 || pipesManager->pipes[pipeId].pipeId == NOT_USED)
    {
        return -1;
    }
    pipe_t * pipe = &pipesManager->pipes[pipeId];
    if((pipe->readIndex) + size >= (pipe->writeIndex))
    {
        size = pipe->writeIndex - pipe->readIndex;
    }
    memcpy(buf, pipe->buffer + pipe->readIndex, size);
    if(pipe->writeIndex == pipe->readIndex){
        pipe->writeIndex = 0;
        pipe->readIndex = 0;
    }
    return size;
}
