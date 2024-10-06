#ifndef _SCHEDULER_H
#define _SCHEDULER_H

#define SCHEDULER_ADDRESS (0x700000)
#define MAX_PROCESSES 70

#include <stdint.h>
#include <process.h>
#include <string.h>




typedef struct schedulerCDT  *schedulerADT;
uint8_t * scheduler(uint8_t * currentSP);
void initScheduler();
uint8_t createProcess(char * name, uint8_t argc, char * args[]);
int32_t kill(int32_t pid);
int32_t block(int32_t pid);
int32_t unblock(int32_t pid);
#endif