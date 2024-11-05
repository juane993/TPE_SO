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
#include <test_sync.h>

#define QTY_PROC 4

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
void run_test_sync(char * arg1, char * arg2, char * arg3);
int filter();
int wc();
int cat();
void run_cat();
//int cat();

//void psPrint();

#endif //TPE_SO_PROCESS_H
