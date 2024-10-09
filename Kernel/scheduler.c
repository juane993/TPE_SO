#include <scheduler.h>

uint8_t currentRunningProceses = 0;
uint8_t currentRoundRobbinPointers = 0;

typedef struct  schedulerCDT{
    uint8_t currentPid;
    uint8_t nextUPid;
    uint8_t currentRoundRobbinIndex;
    PCB procesess[MAX_PROCESSES];
    PCB * roundRobbinPointers[PRIORITY_LEVELS * MAX_PROCESSES]; //suficientemente grande
}schedulerCDT;

schedulerADT sch;
char initialized = 0;

void initScheduler()
{
    sch = (schedulerADT) SCHEDULER_ADDRESS;
    sch->currentPid = 0;
    sch->currentRoundRobbinIndex = 0;
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
    if(currentRunningProceses == 0)
    {
        return currentSP;
    }
    uint8_t currentPid = sch->currentPid;
    sch->procesess[currentPid].RSP = currentSP;
    uint8_t roundRobinIndex = sch->currentRoundRobbinIndex;
    for (int i = roundRobinIndex + 1; i < currentRoundRobbinPointers; i++) {
        if((*(sch->roundRobbinPointers[i])).state == READY)
        {
            sch->currentRoundRobbinIndex = i;
            sch->currentPid = (*(sch->roundRobbinPointers[i])).processPid;
            return (*(sch->roundRobbinPointers[i])).RSP;
        }
    }
       for (int i = 0; i < roundRobinIndex; i++) {
           if((*(sch->roundRobbinPointers[i])).state == READY)
           {
               sch->currentRoundRobbinIndex = i;
               sch->currentPid = (*(sch->roundRobbinPointers[i])).processPid;
               return (*(sch->roundRobbinPointers[i])).RSP;
           }
    }
    return (*(sch->roundRobbinPointers[roundRobinIndex])).RSP;
}

//uint8_t * scheduler(uint8_t * currentSP)
//{
//    if(!initialized)
//    {
//        initScheduler();
//    }
//    if(currentRunningProceses == 0)
//    {
//        return currentSP;
//    }
//   uint8_t currentPid = sch->currentPid;
//   sch->procesess[currentPid].RSP = currentSP;
//   for (int i = currentPid + 1; i < currentRunningProceses; i++) {
//        if(sch->procesess[i].state == READY)
//        {
//            currentPid = i; //innecesario esta asignacion
//            sch->currentPid = i;
//            return sch->procesess[i].RSP;
//           // sch->procesess[i].state = RUNNING;
//        }
//   }
//   for (int i = 0; i < currentPid; i++) {
//       if(sch->procesess[i].state == READY)
//       {
//           currentPid = i;
//           sch->currentPid = i;
//           return sch->procesess[i].RSP;
//       }
//   }
//   return sch->procesess[currentPid].RSP;
//}

int8_t createProcess(void * mainFunc,uint8_t argc, char * args[], Priority prio)
{

    uint8_t newProcessPid = 0;
    if(currentRunningProceses >= MAX_PROCESSES)
    {
        return -1;
    }
    char foundSpace = 0;
    for (int i = 0; i < MAX_PROCESSES && !foundSpace; ++i) {
        if(sch->procesess[i].state == NOT_EXISTS)
        {
            newProcessPid = i;
            foundSpace = 1;
        }
    }
    uint8_t pid = initProcess(&(sch->procesess[newProcessPid]),mainFunc,argc,args,newProcessPid);
    for (int i = currentRoundRobbinPointers; i < currentRoundRobbinPointers + prio; ++i) {
        sch->roundRobbinPointers[i] = &(sch->procesess[newProcessPid]);
    }
    if(pid >= 0) {
        currentRunningProceses++;
        currentRoundRobbinPointers += prio;
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