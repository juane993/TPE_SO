#include <scheduler.h>


uint8_t currentRunningProceses = 0;

typedef struct PCB {
        uint8_t priority;
        uint8_t processPid;
        State state;
        uint8_t * RSP;
} PCB;


typedef struct  schedulerCDT{
    uint8_t currentPid;
    PCB procesess[70];
}schedulerCDT;


schedulerADT sch = (schedulerADT) SCHEDULER_ADDRESS;

uint8_t * scheduler(uint8_t * currentSP)
{
   uint8_t currentPid = sch->currentPid;
   sch->procesess[currentPid].RSP = currentSP;
   sch->procesess[currentPid].state = READY;
   sch->procesess[currentPid + 1% currentRunningProceses].state = RUNNING;
    return sch->procesess[currentPid + 1 % currentRunningProceses].RSP;
    
    //es un proceso nuevo, hay que crearlo
}

/**
*   Dentro del PCB --> Guardamos SP y estado (ready, bloqueado)
*   Debo iterar por una lista de PCB y a los ready los corro y a los que no los salteo
*
*/