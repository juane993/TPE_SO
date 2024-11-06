#ifndef TPE_SO_PIPES_H
#define TPE_SO_PIPES_H

#include <string.h>
#include <lib.h>
#include <defs.h>

#define PIPES_MANAGER_ADDRESS (0x900000)

#define PIPE_BUF_SIZE 100


#define NOT_USED -1

typedef struct pipesCDT * pipesADT;

typedef struct pipe_t{
    int8_t pipeId;
    uint8_t readIndex;
    uint8_t writeIndex;
    uint8_t buffer[PIPE_BUF_SIZE];
}pipe_t;



void initPipeManager();
int8_t initPipe(uint8_t pipeId);
int8_t closePipe(uint8_t pipeId);
int8_t writePipe(uint8_t pipeId, char * buf, uint8_t size);
int8_t readPipe(uint8_t pipeId, char * buf, uint8_t size);

#endif //TPE_SO_PIPES_H
