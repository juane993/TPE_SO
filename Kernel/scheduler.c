#include <scheduler.h>

void deleteProcessPointers(uint32_t pid);
void asignPriority(int32_t pid, Priority newPrio);
void calculateMaxPrio();

uint8_t shellRunning = 0;
typedef struct schedulerCDT
{
    uint8_t currentPid;
    uint8_t nextUPid;

    // con la implementacion esta capaz lo volamos
    uint8_t currentRoundRobbinIndex;

    // el contador que cuenta en que loop estoy del round robbin. yo voy a
    // correr un proceso si es menor a la prioridad del proceso. sino, no corre
    // hasta que se resetee.
    uint8_t roundRobbinLoopCounter;

    // la prioridad maxima actual. inicialmente es 20 por la shell. cuando el
    // contador llegue a este numero termina la iteracion y reinicio el contador
    uint8_t currentMaxPrio;
    PCB procesess[MAX_PROCESSES];

} schedulerCDT;

schedulerADT sch;
char initialized = 0;

void initScheduler()
{
    sch                          = (schedulerADT) SCHEDULER_ADDRESS;
    sch->currentPid              = 0;
    sch->nextUPid                = 0;
    sch->currentMaxPrio          = SHELL;
    sch->currentRoundRobbinIndex = 0;
    sch->roundRobbinLoopCounter  = 0;
    for (int i = 0; i < MAX_PROCESSES; ++i)
    {
        sch->procesess[i].state = NOT_EXISTS;
    }

    initialized = 1;
}

schedulerADT getScheduler()
{
    return sch;
}
PCB * getProcessCurrentPid()
{
    return &(sch->procesess[sch->currentPid]);
}
PCB * getProcess(uint8_t pid)
{
    return &(sch->procesess[pid]);
}
uint8_t getCurrentPid()
{
    return sch->currentPid;
}

uint8_t * getNextReadyProcess(uint8_t start, uint8_t end)
{
    for (int i = start; i < end; ++i)
    {
        if (sch->procesess[i].state == READY && sch->procesess[i].priority > sch->roundRobbinLoopCounter)
        {
            sch->currentRoundRobbinIndex = i;
            sch->currentPid              = i;
            return sch->procesess[i].RSP;
        }
    }
    return NULL;
}

uint8_t * scheduler(uint8_t * currentSP)
{
    if (!initialized)
    {
        initScheduler();
    }
    if (!shellRunning)
    {
        return currentSP;
    }
    uint8_t currentPid             = sch->currentPid;
    sch->procesess[currentPid].RSP = currentSP;
    uint8_t roundRobinIndex        = currentPid;
    uint8_t * nextProcess          = NULL;
    nextProcess                    = getNextReadyProcess(roundRobinIndex + 1, MAX_PROCESSES);
    if (nextProcess != NULL)
    {
        return nextProcess;
    }
    sch->roundRobbinLoopCounter++;
    nextProcess = getNextReadyProcess(0, roundRobinIndex);
    if (nextProcess != NULL)
    {
        return nextProcess;
    }

    // si llego al final del scheduler, quiere decir que me quedo solo un
    // proceso corriendo
    if (sch->roundRobbinLoopCounter >= sch->currentMaxPrio)
    {
        sch->roundRobbinLoopCounter = 0;

        nextProcess = getNextReadyProcess(0, MAX_PROCESSES);
        if (nextProcess != NULL)
        {
            return nextProcess;
        }
    }

    return sch->procesess[roundRobinIndex].RSP;
}

int8_t createProcess(void * mainFunc, uint8_t argc, char * args[], Priority prio, BackgroundState bgState)
{
    uint8_t newProcessPid = 0;
    char foundSpace       = 0;
    for (int i = 0; i < MAX_PROCESSES && !foundSpace; ++i)
    {
        if (sch->procesess[i].state == NOT_EXISTS)
        {
            newProcessPid = i;
            foundSpace    = 1;
        }
    }
    if (!foundSpace)
    {
        return -1;
    }
    uint8_t pid = initProcess(&(sch->procesess[newProcessPid]), mainFunc, argc, args, newProcessPid, prio, bgState, sch->currentPid);
    //    for (int i = currentRoundRobbinPointers; i <
    //    currentRoundRobbinPointers + prio; ++i) {
    //        sch->roundRobbinPointers[i] = &(sch->procesess[newProcessPid]);
    //    }

    asignPriority(pid, prio);
    if (prio == SHELL)
    {
        shellRunning = 1;
    }
    calculateMaxPrio();

    PCB * currentProc = &(sch->procesess[sch->currentPid]);

    currentProc->children[currentProc->childIndex] = &(sch->procesess[pid]);
    currentProc->childIndex++;

    return pid;
}
int32_t kill(int32_t pid)
{
    if (pid == 0 || pid == 1)
    {
        return -1;
    }
    uint32_t value = killProcess(&(sch->procesess[pid]));
    if (value == -1)
    {
        return -1;
    }

    calculateMaxPrio();

    return pid;
}

int8_t waitPid(uint8_t pid)
{
    if (sch->procesess[pid].state == NOT_EXISTS)
    {
        return -1;
    }
    if (sch->procesess[pid].state == ZOMBIE)
    {
        kill(pid);
        return pid;
    }
    sch->procesess[pid].waitingForMe[sch->procesess[pid].waitingIndex].waitingPCB  = getProcessCurrentPid();
    sch->procesess[pid].waitingForMe[sch->procesess[pid].waitingIndex].boolUnblock = TRUE;
    sch->procesess[pid].waitingIndex++;
    block(sch->currentPid);
    return pid;
}

int32_t block(int32_t pid)
{
    if (sch->procesess[pid].state == NOT_EXISTS)
    {
        return -1;
    }
    sch->procesess[pid].state = BLOCK;
    calculateMaxPrio();
    return pid;
}
int32_t zombie(int32_t pid)
{
    if (sch->procesess[pid].state == NOT_EXISTS)
    {
        return -1;
    }
    sch->procesess[pid].state = ZOMBIE;
    calculateMaxPrio();
    return pid;
}

int32_t nice(int32_t pid, Priority newPrio)
{
    if (sch->procesess[pid].state == NOT_EXISTS)
    {
        return -1;
    }
    //    deleteProcessPointers(pid);
    asignPriority(pid, newPrio);
    return pid;
}

void asignPriority(int32_t pid, Priority newPrio)
{
    sch->procesess[pid].priority = newPrio;
    calculateMaxPrio();
}

int32_t unblock(int32_t pid)
{
    if (sch->procesess[pid].state == NOT_EXISTS)
    {
        return -1;
    }
    if (sch->procesess[pid].state == READY)
    {
        return pid;
    }
    sch->procesess[pid].state = READY;
    calculateMaxPrio();
    return pid;
}

void calculateMaxPrio()
{
    char shellPrioFound = 0;
    uint8_t max         = 0;
    for (int i = 0; i < MAX_PROCESSES && !shellPrioFound; ++i)
    {
        if (sch->procesess[i].state == READY && sch->procesess[i].priority > max)
        {
            max = sch->procesess[i].priority;
            if (max == SHELL)
            {
                shellPrioFound = 1;
            }
        }
    }
    sch->currentMaxPrio = max;
}

char * ps()
{
    char * buffer        = (char *) allocMemory(1000);
    char * currentBufPos = buffer;
    char itoaBuffer[10]  = {0};
    for (int i = 0; i < MAX_PROCESSES; ++i)
    {
        if (sch->procesess[i].state != NOT_EXISTS)
        {
            strcpy(currentBufPos, "PID: ");
            currentBufPos += 5;
            char len = itoa(i, itoaBuffer, 10);
            strcpy(currentBufPos, itoaBuffer);
            currentBufPos += len;
            strcpy(currentBufPos, ", State: ");
            currentBufPos += 9;
            len = itoa(sch->procesess[i].state, itoaBuffer, 10);
            strcpy(currentBufPos, itoaBuffer);
            currentBufPos += len;
            strcpy(currentBufPos, ", Priority: ");
            currentBufPos += 12;
            len = itoa(sch->procesess[i].priority, itoaBuffer, 10);
            strcpy(currentBufPos, itoaBuffer);
            currentBufPos += len;
            strcpy(currentBufPos, ", BasePointer: ");
            currentBufPos += 15;
            len = itoa((uintptr_t) sch->procesess[i].stackBase, itoaBuffer, 16);
            strcpy(currentBufPos, itoaBuffer);
            currentBufPos += len;
            strcpy(currentBufPos, ", StackPointer: ");
            currentBufPos += 16;
            len = itoa((uintptr_t) sch->procesess[i].RSP, itoaBuffer, 16);
            strcpy(currentBufPos, itoaBuffer);
            currentBufPos += len;
            strcpy(currentBufPos, ", BGState: ");
            currentBufPos += 11;
            len = itoa(sch->procesess[i].bgState, itoaBuffer, 10);
            strcpy(currentBufPos, itoaBuffer);
            currentBufPos += len;
            *currentBufPos = '\n';
            currentBufPos++;
        }
    }
    return buffer;
}

int8_t my_get_pid()
{
    return sch->currentPid;
}

void unblockWaiters(uint8_t pid)
{
    char found          = 0;
    uint8_t waitingInex = sch->procesess[pid].waitingIndex;
    for (int i = 0; i < waitingInex; ++i)
    {
        WaitingProcess * proc = &sch->procesess[pid].waitingForMe[i];
        if (proc->waitingPCB != NULL && proc->waitingPCB->state != NOT_EXISTS)
        {
            if (proc->boolUnblock)
            {
                unblock(proc->waitingPCB->processPid);
            }

            found = 1;
        }
    }
    if (found)
    {
        kill(pid);
    }
    calculateMaxPrio();
}

int8_t waitPidNoHang(uint8_t pid)
{
    if (sch->procesess[pid].state == NOT_EXISTS)
    {
        return -1;
    }
    if (sch->procesess[pid].state == ZOMBIE)
    {
        kill(pid);
        return pid;
    }
    sch->procesess[pid].waitingForMe[sch->procesess[pid].waitingIndex].waitingPCB  = getProcessCurrentPid();
    sch->procesess[pid].waitingForMe[sch->procesess[pid].waitingIndex].boolUnblock = FALSE;
    sch->procesess[pid].waitingIndex++;
    return pid;
}

void adoptOrphans(uint8_t parentPid)
{

    // OJO COM EL CHILDREN
    PCB ** children    = (sch->procesess[parentPid].children);
    uint8_t childIndex = sch->procesess[parentPid].childIndex;

    for (int i = 0; i < childIndex; ++i)
    {
        uint8_t waitingIndex                                = children[i]->waitingIndex;
        children[i]->waitingForMe[waitingIndex].waitingPCB  = &(sch->procesess[1]); // Now Shell waits for ME
        children[i]->waitingForMe[waitingIndex].boolUnblock = FALSE;
        children[i]->waitingIndex++;
    }
}

void unblockFDWaiter(uint8_t fd)
{
    for (int i = 0; i < MAX_PROCESSES; ++i)
    {
        if (sch->procesess[i].waitingForFD == fd)
        {
            unblock(i);
            sch->procesess[i].waitingForFD = EMPTY_FD;
            return;
        }
    }
}

/**
 *   Dentro del PCB --> Guardamos SP y estado (ready, bloqueado)
 *   Debo iterar por una lista de PCB y a los ready los corro y a los que no los
 * salteo
 *
 */