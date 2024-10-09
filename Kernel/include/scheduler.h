#ifndef _SCHEDULER_H
#define _SCHEDULER_H

#define SCHEDULER_ADDRESS (0x700000)
#define MAX_PROCESSES 70
#define PRIORITY_LEVELS 3

#include <stdint.h>
#include <process.h>
#include <string.h>

typedef enum {
  LOW = 1,
  MEDIUM = 5,
  HIGH = 10,
  SHELL = 20
} Priority;

typedef struct schedulerCDT  *schedulerADT;
uint8_t * scheduler(uint8_t * currentSP);
void initScheduler();
int8_t createProcess(void * name, uint8_t argc, char * args[], Priority prio);
int32_t kill(int32_t pid);
int32_t block(int32_t pid);
int32_t unblock(int32_t pid);
#endif