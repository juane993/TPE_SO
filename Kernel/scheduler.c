#include <scheduler.h>


uint8_t currentRunningProceses = 1;

typedef struct PCB {
        uint8_t priority;
        uint8_t processPid;
        State state;
        uint8_t * RSP;
} PCB;


typedef struct  schedulerCDT{
    uint8_t currentPid;
    uint8_t nextUPid;
    PCB procesess[MAX_PROCESSES];
}schedulerCDT;


schedulerADT sch;

void initScheduler()
{
    sch = (schedulerADT) SCHEDULER_ADDRESS;
    for (int i = 0; i < MAX_PROCESSES; ++i) {
        sch->procesess[i].state = NOT_EXISTS;
    }
}

uint8_t * scheduler(uint8_t * currentSP)
{
    return currentSP;
   uint8_t currentPid = sch->currentPid;
   sch->procesess[currentPid].RSP = currentSP;


   for (int i = currentPid + 1; i < currentRunningProceses; i++) {
        if(sch->procesess[i].state == READY)
        {
            currentPid = i;
            return sch->procesess[i].RSP;
           // sch->procesess[i].state = RUNNING;
        }
   }
   for (int i = 0; i < currentPid; i++) {
       if(sch->procesess[i].state == READY)
       {
           currentPid = i;
           return sch->procesess[i].RSP;
       }
   }

   return currentSP;
}

uint8_t createProcess(void * entryPoint, char ** args, uint8_t argc)
{
    //RSP = malloc;
    //pushFlags, entryPoint, bp
    //pushstate
    //processes[currentRunningProcesess].state = ready, procesPid = currentRuningProcess
    //currentRunningProcesse++
    //int 20
    //
    return -1;
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