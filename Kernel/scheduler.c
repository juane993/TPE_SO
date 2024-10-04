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

schedulerADT scheduler;
//schedulerADT scheduler = (schedulerADT) SCHEDULER_ADDRESS;

uint8_t * schedulerrrrr(uint8_t * current)
{
    return current;
    for (int i = 0; i < currentRunningProceses; ++i) {
        if(current == scheduler->procesess[i].RSP)
        {
            scheduler->procesess[i].state = READY;
            scheduler->procesess[i+1 % currentRunningProceses].state = RUNNING;
            return scheduler->procesess[i+1 % currentRunningProceses].RSP; //hay que chequear si esta bloqueado
        }
    }
    //es un proceso nuevo, hay que crearlo
}

/**
*   Dentro del PCB --> Guardamos SP y estado (ready, bloqueado)
*   Debo iterar por una lista de PCB y a los ready los corro y a los que no los salteo
*
*/