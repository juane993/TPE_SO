#ifndef TPE_SO_TEST_SYNC_H
#define TPE_SO_TEST_SYNC_H

#include <stdint.h>
#include <stdio.h>
#include <syscalls.h>
#include <test_util.h>
#include <semaphore.h>
#include <string.h>

#define SEM_ID               1
#define TOTAL_PAIR_PROCESSES 4

int64_t test_sync(int argc, char * argv[]);

#endif // TPE_SO_TEST_SYNC_H