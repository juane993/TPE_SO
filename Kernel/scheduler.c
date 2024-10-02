#include <scheduler.h>

PCB procesess[70];
uint8 currentRunningProceses = 0;

uint8 * scheduler(uint8 * current)
{
    for (int i = 0; i < currentRunningProceses; ++i) {
        if(current == procesess[i]->RSP)
        {
            procesess[i]->state = READY;
            procesess[i+1 % currentRunningProceses] -> state = RUNNING;
            return procesess[i+1 % currentRunningProceses] -> RSP; //hay que chequear si esta bloqueado
        }
    }
    //es un proceso nuevo, hay que crearlo
}

/**
*   Dentro del PCB --> Guardamos SP y estado (ready, bloqueado)
*   Debo iterar por una lista de PCB y a los ready los corro y a los que no los salteo
*
*/