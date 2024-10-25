#ifndef _SCHEDULER_H
#define _SCHEDULER_H

#define SCHEDULER_ADDRESS (0x700000)
#define MAX_PROCESSES 70
//#define PRIORITY_LEVELS 10
#define MAX_PRIO_LEVELS 10 //cuando usamos esto le sumamo 10 porque la shell es el unico
//proceso que tiene prioridad 20. la suma es para que las cuentas den bien y no nos pasemos


#include <stdint.h>
#include <process.h>
#include <string.h>
#include <defs.h>
#include <lib.h>
#include <MemoryManager.h>
typedef struct schedulerCDT  *schedulerADT;
uint8_t * scheduler(uint8_t * currentSP);
void initScheduler();
int8_t createProcess(void * name, uint8_t argc, char * args[], Priority prio, BackgroundState bgState);
int32_t kill(int32_t pid);
int32_t block(int32_t pid);
int32_t zombie(int32_t pid);
int32_t unblock(int32_t pid);
int32_t nice(int32_t pid, Priority newPrio);
void my_exit();
char * ps();
int8_t my_get_pid();
schedulerADT getScheduler();
PCB * getProcessCurrentPid();
PCB * getProcess(uint8_t pid);
uint8_t getCurrentPid();

#endif