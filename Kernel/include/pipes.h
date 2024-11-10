#ifndef TPE_SO_PIPES_H
#define TPE_SO_PIPES_H

#include <string.h>
#include <lib.h>
#include <defs.h>

#define PIPES_MANAGER_ADDRESS (0x900000)

#define PIPE_BUF_SIZE 100

#define NOT_USED -1

typedef struct pipesCDT * pipesADT;

typedef struct pipe_t
{
    // fd[0] = read
    // fd[1] = write
    uint8_t readEndClosed;
    uint8_t writeEndClosed;
    int8_t pipeFDs[2];
    int8_t pipeId;
    uint8_t readIndex;
    uint8_t writeIndex;
    uint8_t writePid;
    uint8_t readPid;
    uint8_t buffer[PIPE_BUF_SIZE];
} pipe_t;

void initPipeManager();
int8_t initPipe(int8_t * pipeFDs);
int8_t closePipeEnd(int8_t pipeFD);
int8_t writePipe(int8_t pipeFD, char * buf, uint8_t size);
int8_t readPipe(int8_t pipeFD, char * buf, uint8_t size);

#endif // TPE_SO_PIPES_H
