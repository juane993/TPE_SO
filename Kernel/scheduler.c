#include <scheduler.h>

uint8_t currentRunningProceses = 0;

typedef struct  schedulerCDT{
    uint8_t currentPid;
    uint8_t nextUPid;
    PCB procesess[MAX_PROCESSES];
}schedulerCDT;


schedulerADT sch;
char initialized = 0;

void initScheduler()
{
    sch = (schedulerADT) SCHEDULER_ADDRESS;
    sch->currentPid = 0;
    sch->nextUPid = 0;
    for (int i = 0; i < MAX_PROCESSES; ++i) {
        sch->procesess[i].state = NOT_EXISTS;
    }
    initialized = 1;
}

uint8_t * scheduler(uint8_t * currentSP)
{
    if(!initialized)
    {
        initScheduler();
    }
   uint8_t currentPid = sch->currentPid;
//    if(currentRunningProceses>1)
//    {
//        sch->procesess[currentPid].RSP = currentSP;
//    }
   for (int i = currentPid + 1; i < currentRunningProceses; i++) {
        if(sch->procesess[i].state == READY)
        {
            currentPid = i; //innecesario esta asignacion
            sch->currentPid = i;
            return sch->procesess[i].RSP;
           // sch->procesess[i].state = RUNNING;
        }
   }
   for (int i = 0; i < currentPid; i++) {
       if(sch->procesess[i].state == READY)
       {
           currentPid = i;
           sch->currentPid = i;
           return sch->procesess[i].RSP;
       }
   }
   return currentSP;
}

int8_t createProcess(void * mainFunc,uint8_t argc, char * args[])
{
    uint8_t pid = initProcess(&(sch->procesess[currentRunningProceses]),mainFunc,argc,args,currentRunningProceses);

    if(pid >= 0) {
        currentRunningProceses++;
    }
    return pid;
}

int32_t kill(int32_t pid){
    return -1;
}
int32_t block(int32_t pid){
    return -1;
}
int32_t unblock(int32_t pid){
    return -1;
}



/**
*   Dentro del PCB --> Guardamos SP y estado (ready, bloqueado)
*   Debo iterar por una lista de PCB y a los ready los corro y a los que no los salteo
*
*/