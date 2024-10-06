#include <scheduler.h>

uint8_t currentRunningProceses = 1;

typedef struct  schedulerCDT{
    uint8_t currentPid;
    uint8_t nextUPid;
    PCB procesess[MAX_PROCESSES];
    processNames[1];
}schedulerCDT;


schedulerADT sch;

void initScheduler()
{
    sch = (schedulerADT) SCHEDULER_ADDRESS;
    for (int i = 0; i < MAX_PROCESSES; ++i) {
        sch->procesess[i].state = NOT_EXISTS;
    }
    sch->processNames[0]->name = "endless_loop";
    sch->processNames[0]->mainFunc = 0; //puntero a la funcion del proceso
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

uint8_t createProcess(char * name,uint8_t argc, char ** args, )
{
    void * mainFunc;
    char found = 0;
    for (int i = 0; i < 1 && !found; ++i) {
        if(strcmp(name,sch->procesessNames[i]->name) == 0)
        {
            mainFunc = sch->procesessNames[i]->mainFunc;
            found = 1;
        }
    }
    uint64 * initProcess(mainFunc,argc,args);
    sch->processes[currentRunningProcesess].state = ready;
    sch->procesess[currentRunningProcesess].PCB->processPid = currentRunningProceses;
    //currentRunningProcesse++
    //sch->procesess[currentPid].RSP = currentSP;
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