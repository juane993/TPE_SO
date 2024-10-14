#ifndef TPE_SO_PROCESS_H
#define TPE_SO_PROCESS_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <syscalls.h>
#include <test_util.h>
#include <test_prio.h>
#include <test_processes.h>

#define QTY_PROC 3

typedef struct procesess{
    char * name;
    void * entryPoint;
}procesess;

typedef enum {
    BACKGROUND = 0,
    FOREGROUND = 1
} BackgroundState;


int8_t runProcess(char *name,char * bgState, char *argv[]);
int8_t kill(char *pid);

void initProcesess();
void run_test_proc(char * argv);
void run_test_prio();

//void psPrint();

#endif //TPE_SO_PROCESS_H
