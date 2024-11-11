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
#include <test_mm.h>
#include <libasm.h>
#include <philo.h>

#define QTY_PROC 4

typedef struct procesess
{
    char * name;
    void * entryPoint;
} procesess;

typedef enum
{
    BACKGROUND = 0,
    FOREGROUND = 1
} BackgroundState;

int8_t runProcess(char * name, char * bgState, char * argv[]);
int8_t kill(char * pid);

void initProcesess();
int8_t run_test_proc(char * argv, char * bgState);
int8_t run_test_prio(char * bgState);
int8_t run_test_sync(char * arg1, char * arg2, char * arg3, char * bgState);
int8_t filter();
int8_t wc();
int8_t cat();
int8_t loop();
int8_t run_cat(char * bgState);
int8_t run_wc(char * bgState);
int8_t run_filter(char * bgState);
int8_t run_loop(char * arg1, char * bgState);
int8_t run_idle_process();
int8_t run_test_mem(char * arg1, char * bgState);
int8_t run_philo(char * bgState);
int8_t validate_bg_state(char * bgState);


// void psPrint();

#endif // TPE_SO_PROCESS_H
