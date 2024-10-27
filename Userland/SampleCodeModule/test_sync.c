#include <test_sync.h>



int64_t global; // shared memory

void slowInc(int64_t *p, int64_t inc) {
//    if(*p < 0)
//       // printf( "NEGATIVE by process with pid: %d before.\n",my_getPid());
//    else
       // printf( "%d by process with pid: %d before. I will %s \n", *p,my_getPid(),inc>0?"increment":"decrement");
  int64_t aux = *p;
  my_yield(); // This makes the race condition highly probable
  aux += inc;
  *p = aux;
//    if(*p < 0)
//        //printf( "NEGATIVE by process with pid: %d after \n",my_getPid());
//    else
        //printf( "%d by process with pid: %d after. %s \n", *p,my_getPid(),inc>0?"i incremented":"i decremented");

}

uint64_t my_process_inc(uint64_t argc, char *argv[]) {
    char pid = my_getPid();


  uint64_t n;
  int8_t inc;
  int8_t use_sem;

  if (argc != 3){

      return -1;
  }


    if ((n = satoi(argv[0])) <= 0)
    {

        return -1;
    }
    if ((inc = satoi(argv[1])) == 0)
    {

        return -1;
    }

    if ((use_sem = satoi(argv[2])) < 0)
    {

        return -1;
    }


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
    return 0;
}



int64_t test_sync(int argc, char *argv[]) { //{n, use_sem, 0}

  uint64_t pids[2 * TOTAL_PAIR_PROCESSES];

  if (argc != 3){

      return -1;
  }

//  shellWaitTest();
//   return 17;



  char decrement[5];
  decrement[0] = '-';
  strcpy(decrement + 1, argv[1]);


  char *argvDec[] = {argv[0], decrement,argv[2], NULL};
  char *argvInc[] = {argv[0], argv[1],argv[2], NULL};

  global = 0;

  uint64_t i;
  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    pids[i] = my_create_process(my_process_inc, 3, argvInc, LOW,0);
    pids[i + TOTAL_PAIR_PROCESSES] = my_create_process(my_process_inc, 3, argvDec,LOW,0);

  }




  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    my_waitPid(pids[i]);
      printf("waiting for children with pid: %d\n",pids[0]);
    my_waitPid(pids[i + TOTAL_PAIR_PROCESSES]);
      printf("waiting for children with pid: %d\n",pids[i + TOTAL_PAIR_PROCESSES]);
  }

  if(global >= 0)
    printf("Final value: %d\n", global);
  else
    printf("NEGATIVE Final value: -%d\n", global * -1);

  return 0;
}
