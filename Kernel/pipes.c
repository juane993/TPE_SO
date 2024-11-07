#include <pipes.h>
#define PIPE_INDEX(pipeFD) (((pipeFD) - 4) / 2)
#define INVALID_READ_PIPE_FD(pipeFD) ((pipeFD) < 4 || (pipeFD) % 2 != 0 || (pipeFD) > 2 * MAX_PIPES + 5)
#define INVALID_WRITE_PIPE_FD(pipeFD) ((pipeFD) < 4 || (pipeFD) % 2 == 0 || (pipeFD) > 2 * MAX_PIPES + 5)


typedef struct pipesCDT{
    pipe_t pipes[MAX_PIPES];
    uint8_t pipeIndex;
} pipesCDT;

pipesADT pipesManager;

void initPipeManager()
{
    pipesManager = (pipesADT) PIPES_MANAGER_ADDRESS;
    pipesManager->pipeIndex = 0;
    int j = 4;
    for (uint8_t i = 0; i < MAX_PIPES; i++) {
        pipesManager->pipes[i].pipeId = NOT_USED;
        pipesManager->pipes[i].pipeFDs[0] = j;
        j++;
        pipesManager->pipes[i].pipeFDs[1] = j;
        j++;
    }
}
/* pipe0 = 4,5      4-4 = 0                5-4 = 0
 * pipe1 = 6,7      6-4 = 2 2/2=1          7-4 = 3   3/2 = 1
 * pipe2 = 8,9      8-4 = 4 4/2=2          9-4 = 5/2 = 2
 * pipe3 = 10,11    10-4 = 6 6/2=3         11-4=7   7/2 = 3
 * pipe4 = 12,13    12 - 4 = 8 8/2 = 4     13-4 = 9  9/2 = 4
 * pipe5 = 14,15    14 - 4 = 10 10/2 = 5   15-4 = 11 11/2 = 5
 * pipe6 = 16,17
 */


int8_t initPipe(int8_t * pipeFDs){
    uint8_t pipeId;
    char foundSpace = 0;
    for (int i = 0; i < MAX_PIPES && !foundSpace; ++i) {
        if(pipesManager->pipes[i].pipeId == NOT_USED){
            pipeId = i;
            foundSpace = 1;
        }
    }
    if(!foundSpace){
        return -1;
    }
    pipeFDs[0] = pipesManager->pipes[pipeId].pipeFDs[0];
    pipeFDs[1] = pipesManager->pipes[pipeId].pipeFDs[1];
    pipesManager->pipes[pipeId].pipeId = pipeId;
    pipesManager->pipes[pipeId].buffer[0] = 0;
    pipesManager->pipes[pipeId].readIndex = 0;
    pipesManager->pipes[pipeId].writeIndex = 0;
    pipesManager->pipes[pipeId].readEndClosed = 0;
    pipesManager->pipes[pipeId].writeEndClosed = 0;
    return 0;
}

int8_t closePipeEnd(int8_t pipeFD){
       if(pipeFD < 4 || pipeFD > 2*MAX_PIPES + 5){
             return -1;
       }

    int8_t pipeId = PIPE_INDEX(pipeFD);
    if(pipeFD % 2 == 0)
    {
        pipesManager->pipes[pipeId].readEndClosed = 1;
    }
    else
    {
        pipesManager->pipes[pipeId].writeEndClosed = 1;
    }
    if(pipesManager->pipes[pipeId].readEndClosed && pipesManager->pipes[pipeId].writeEndClosed)
    {
        pipesManager->pipes[pipeId].pipeId = NOT_USED;
    }
    return pipeId;
}

int8_t writePipe(int8_t pipeFD, char * buf, uint8_t size){
    //pipeId = pipeId-4;
    if(INVALID_WRITE_PIPE_FD(pipeFD)){
        return -1;
    }
    int8_t pipeId = PIPE_INDEX(pipeFD);

    if(pipesManager->pipes[pipeId].pipeId == NOT_USED)
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

int8_t readPipe(int8_t pipeFD, char * buf, uint8_t size){
    // pipeId = pipeId-4;

   if(INVALID_READ_PIPE_FD(pipeFD)){
       return -1;
   }
    int8_t pipeId = PIPE_INDEX(pipeFD);

    if(pipesManager->pipes[pipeId].pipeId == NOT_USED)
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
    } else{
        pipe->readIndex += size;
    }
    return size;
}
