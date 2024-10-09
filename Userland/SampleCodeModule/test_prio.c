
#include <test_prio.h>

#define MINOR_WAIT 1000000 // TODO: Change this value to prevent a process from flooding the screen
#define WAIT 10000000      // TODO: Change this value to make the wait long enough to see theese processes beeing run at least twice

#define TOTAL_PROCESSES 3
#define LOWEST 0  // TODO: Change as required
#define MEDIUM 5  // TODO: Change as required
#define HIGHEST 10 // TODO: Change as required

int64_t prio[TOTAL_PROCESSES] = {LOWEST, MEDIUM, HIGHEST};


void bussy_wait2(uint64_t n) {
    uint64_t i;
    for (i = 0; i < n; i++)
        ;
}

void endless_loop_print2(uint64_t wait) {
    //int64_t pid = my_getpid();
    int64_t pid = 3;

    while (1) {
        printf("%d ", pid);
        bussy_wait2(wait);
    }
}



void test_prio() {
   
      int64_t pids[TOTAL_PROCESSES];
      char *argv[] = {0};
      uint64_t i;

      for (i = 0; i < TOTAL_PROCESSES; i++)
        pids[i] = my_create_process(endless_loop_print2, 0, argv,LOW);

      bussy_wait2(WAIT);
      printf("\nCHANGING PRIORITIES...\n");

      for (i = 0; i < TOTAL_PROCESSES; i++)
        my_nice(pids[i], prio[i]);

      bussy_wait2(WAIT);
      printf("\nBLOCKING...\n");

      for (i = 0; i < TOTAL_PROCESSES; i++)
        my_block(pids[i]);

      printf("CHANGING PRIORITIES WHILE BLOCKED...\n");

      for (i = 0; i < TOTAL_PROCESSES; i++)
        my_nice(pids[i], MEDIUM);

      printf("UNBLOCKING...\n");

      for (i = 0; i < TOTAL_PROCESSES; i++)
        my_unblock(pids[i]);

      bussy_wait2(WAIT);
      printf("\nKILLING...\n");

      for (i = 0; i < TOTAL_PROCESSES; i++)
        my_kill(pids[i]);

        printf("\nTEST FINISHED\n");

}
