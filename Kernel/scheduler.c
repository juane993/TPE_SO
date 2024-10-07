#include <scheduler.h>

uint8_t currentRunningProceses = 1;

typedef struct  schedulerCDT{
    uint8_t currentPid;
    uint8_t nextUPid;
    PCB procesess[MAX_PROCESSES];
    processNames names[1];
}schedulerCDT;


schedulerADT sch;

void initScheduler()
{
    sch = (schedulerADT) SCHEDULER_ADDRESS;
    for (int i = 0; i < MAX_PROCESSES; ++i) {
        sch->procesess[i].state = NOT_EXISTS;
    }
    sch->names[0].name = "endless_loop";
    sch->names[0].mainFunc = (void *)0; //puntero a la funcion del proceso
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

int8_t createProcess(char * name,uint8_t argc, char * args[])
{

    void * mainFunc;
    char found = 0;
    for (int i = 0; i < 1 && !found; ++i) {
        char * auxi = sch->names[i].name;
        if(strcmp(name,sch->names[i].name) == 0)
        {
            mainFunc = sch->names[i].mainFunc;
            found = 1;
        }
    }
    if(!found){
        return -1;
    }

    uint64_t * base;
    uint64_t * stackPointer = initProcess(mainFunc,argc,args, &base);
    if(stackPointer == NULL){
        return -1;
    }
    sch->procesess[currentRunningProceses].state = READY;
    sch->procesess[currentRunningProceses].processPid = currentRunningProceses;
    sch->procesess[currentRunningProceses].RSP = stackPointer;
    sch->procesess[currentRunningProceses].stackBase = base;
    sch->procesess[currentRunningProceses].argc = argc;
    sch->procesess[currentRunningProceses].argv = args;
    currentRunningProceses++;
    return sch->procesess[currentRunningProceses].processPid;
    //sch->procesess[currentPid].RSP = currentSP;
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