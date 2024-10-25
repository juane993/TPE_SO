#include <stdint.h>
#include <stdio.h>
#include <syscalls.h>
#include <test_util.h>
#include <semaphore.h>

#define SEM_ID 1
#define TOTAL_PAIR_PROCESSES 2

int64_t global; // shared memory

void slowInc(int64_t *p, int64_t inc) {
    if(*p < 0)
        printf( "NEGATIVE by process with pid: %d before \n",my_getPid());
    else
        printf( "%d by process with pid: %d before \n", *p,my_getPid());
  int64_t aux = *p;
  my_yield(); // This makes the race condition highly probable
  aux += inc;
  *p = aux;
    if(*p < 0)
        printf( "NEGATIVE by process with pid: %d after \n",my_getPid());
    else
        printf( "%d by process with pid: %d after \n", *p,my_getPid());
}

uint64_t my_process_inc(uint64_t argc, char *argv[]) {
  uint64_t n;
  int8_t inc;
  int8_t use_sem;

  if (argc != 4){
      my_exit();
      return -1;
  }


    if ((n = satoi(argv[0])) <= 0)
    {
        my_exit();
        return -1;
    }
    if ((inc = satoi(argv[1])) == 0)
    {
        my_exit();
        return -1;
    }

    if ((use_sem = satoi(argv[3])) < 0)
    {
        my_exit();
        return -1;
    }
    char pid = my_getPid();

  if (use_sem)
    my_semInit(SEM_ID, 1);

  uint64_t i;
  for (i = 0; i < n; i++) {
    if (use_sem)
      my_semWait(SEM_ID);
    slowInc(&global, inc);
    if (use_sem)
      my_semPost(SEM_ID);
  }

//  if (use_sem)
//    my_sem_close(SEM_ID);
    printf("Process %d finished\n", pid);
    my_exit();
  return 0;
}

uint64_t test_sync(int argc, char *argv[]) { //{n, use_sem, 0}
  uint64_t pids[2 * TOTAL_PAIR_PROCESSES];

  if (argc != 3){
      my_exit();
      return -1;
  }


  char *argvDec[] = {argv[0], "-1", argv[1],argv[2], NULL};
  char *argvInc[] = {argv[0], "1", argv[1],argv[2], NULL};

  global = 0;

  uint64_t i;
  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    pids[i] = my_create_process(my_process_inc, 4, argvDec, LOW,0);
    pids[i + TOTAL_PAIR_PROCESSES] = my_create_process(my_process_inc, 4, argvInc,LOW,0);
  }

  //falta implementar / to implement
//  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
//    my_wait(pids[i]);
//    my_wait(pids[i + TOTAL_PAIR_PROCESSES]);
//  }

  printf("Final value: %d\n", global);
my_exit();
  return 0;
}
