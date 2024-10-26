#include <scheduler.h>

void deleteProcessPointers(uint32_t pid);
void asignPriority(int32_t pid, Priority newPrio);


uint16_t currentRoundRobbinPointers = 0;
uint8_t shellRunning = 0;
typedef struct  schedulerCDT{
    uint8_t currentPid;
    uint8_t nextUPid;
    uint8_t currentRoundRobbinIndex;
    PCB procesess[MAX_PROCESSES];
    PCB * roundRobbinPointers[MAX_PRIO_LEVELS * MAX_PROCESSES + 10]; //suficientemente grande
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
    for (int i = 0; i < MAX_PRIO_LEVELS * MAX_PROCESSES + 10; ++i) {
        sch->roundRobbinPointers[i] = NULL;
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

uint8_t * scheduler(uint8_t * currentSP)
{
    if(!initialized)
    {
        initScheduler();
    }
    if(!shellRunning)
    {
        return currentSP;
    }
    uint8_t currentPid = sch->currentPid;
    sch->procesess[currentPid].RSP = currentSP;
    uint8_t roundRobinIndex = sch->currentRoundRobbinIndex;
    for (int i = roundRobinIndex + 1; i < currentRoundRobbinPointers; i++) {
        if(sch->roundRobbinPointers[i] != NULL && (*(sch->roundRobbinPointers[i])).state == READY)
        {
            sch->currentRoundRobbinIndex = i;
            sch->currentPid = (*(sch->roundRobbinPointers[i])).processPid;
            return (*(sch->roundRobbinPointers[i])).RSP;
        }
    }
       for (int i = 0; i < roundRobinIndex; i++) {
           if(sch->roundRobbinPointers[i] != NULL &&(*(sch->roundRobbinPointers[i])).state == READY)
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

int8_t createProcess(void * mainFunc,uint8_t argc, char * args[], Priority prio, BackgroundState bgState)
{
    uint8_t newProcessPid = 0;
    char foundSpace = 0;
    for (int i = 0; i < MAX_PROCESSES && !foundSpace; ++i) {
        if(sch->procesess[i].state == NOT_EXISTS)
        {
            newProcessPid = i;
            foundSpace = 1;
        }
    }
    if(!foundSpace){
        return -1;
    }
    uint8_t pid = initProcess(&(sch->procesess[newProcessPid]),mainFunc,argc,args,newProcessPid,prio, bgState,sch->currentPid);
//    for (int i = currentRoundRobbinPointers; i < currentRoundRobbinPointers + prio; ++i) {
//        sch->roundRobbinPointers[i] = &(sch->procesess[newProcessPid]);
//    }

    asignPriority(pid,prio);
    if(pid >= 0 && currentRoundRobbinPointers < MAX_PROCESSES * MAX_PRIO_LEVELS + 10) {
        currentRoundRobbinPointers += prio;
    }
    if(prio == SHELL){
        shellRunning = 1;
    }
    return pid;
}
int32_t kill(int32_t pid){
    if(pid == 0){
        return -1;
    }
    uint32_t value = killProcess(&(sch->procesess[pid]));
    if(value == -1){
        return -1;
    }
    deleteProcessPointers(pid);

    return pid;
}
void deleteProcessPointers(uint32_t pid)
{
    for (int i = 0; i < MAX_PROCESSES * MAX_PRIO_LEVELS + 10; ++i) {
        if(sch->roundRobbinPointers[i] == &(sch->procesess[pid]))
        {
            sch->roundRobbinPointers[i] = NULL;
        }
    }
}
int32_t block(int32_t pid){
    if(sch->procesess[pid].state == NOT_EXISTS)
    {
        return -1;
    }
    sch->procesess[pid].state = BLOCK;
    return pid;
}
int32_t zombie(int32_t pid){
    if(sch->procesess[pid].state == NOT_EXISTS)
    {
        return -1;
    }
    sch->procesess[pid].state = ZOMBIE;
    return pid;
}

int32_t nice(int32_t pid, Priority newPrio)
{
   if(sch->procesess[pid].state == NOT_EXISTS){
       return -1;
   }
    deleteProcessPointers(pid);
    asignPriority(pid,newPrio);
    return pid;
}

void asignPriority(int32_t pid, Priority newPrio)
{
    int ptrsLeftToAssign = newPrio;
    for (int i = 0; i < MAX_PROCESSES * MAX_PRIO_LEVELS + 10 && ptrsLeftToAssign > 0; ++i) {
        if(sch->roundRobbinPointers[i] == NULL)
        {
            sch->roundRobbinPointers[i] = &(sch->procesess[pid]);
            ptrsLeftToAssign--;
        }
    }
}

void my_exit()
{
    kill(sch->currentPid);
    yield();
}

int32_t unblock(int32_t pid){
    if(sch->procesess[pid].state == NOT_EXISTS)
    {
        return -1;
    }
    sch->procesess[pid].state = READY;
    return pid;
}

char * ps()
{
    char * buffer = (char *) allocMemory(1000);
    char * currentBufPos = buffer;
    char itoaBuffer[10] = {0};
    for (int i = 0; i < MAX_PROCESSES; ++i) {
        if(sch->procesess[i].state != NOT_EXISTS)
        {
            strcpy(currentBufPos,"PID: ");
            currentBufPos+= 5;
            char len = itoa(i,itoaBuffer,10);
            strcpy(currentBufPos,itoaBuffer);
            currentBufPos+=len;
            strcpy(currentBufPos,", State: ");
            currentBufPos+= 9;
            len = itoa(sch->procesess[i].state,itoaBuffer,10);
            strcpy(currentBufPos,itoaBuffer);
            currentBufPos+=len;
            strcpy(currentBufPos,", Priority: ");
            currentBufPos+= 12;
            len = itoa(sch->procesess[i].priority,itoaBuffer,10);
            strcpy(currentBufPos,itoaBuffer);
            currentBufPos+=len;
            strcpy(currentBufPos,", BasePointer: ");
            currentBufPos+= 15;
            len = itoa(sch->procesess[i].stackBase,itoaBuffer,16);
            strcpy(currentBufPos,itoaBuffer);
            currentBufPos+=len;
            strcpy(currentBufPos,", StackPointer: ");
            currentBufPos+= 16;
            len = itoa(sch->procesess[i].RSP,itoaBuffer,16);
            strcpy(currentBufPos,itoaBuffer);
            currentBufPos+=len;
            strcpy(currentBufPos,", BGState: ");
            currentBufPos+= 11;
            len = itoa(sch->procesess[i].bgState,itoaBuffer,10);
            strcpy(currentBufPos,itoaBuffer);
            currentBufPos+=len;
            *currentBufPos = '\n';
            currentBufPos++;

        }
    }
    return buffer;
}

int8_t my_get_pid(){
    return sch->currentPid;
}


/// TODO implement
//void waitForChildren()
//{
//    for (int i = 1; i < MAX_PROCESSES; ++i) {
//        sch->procesess[i].waitingForMe[sch->procesess[i].waitingIndex] = getProcessCurrentPid();
//        sch->procesess[i].waitingIndex++;
//    }
//}

int8_t waitPid(uint8_t pid)
{
    if(sch->procesess[pid].state == NOT_EXISTS)
    {
        return -1;
    }
    if(sch->procesess[pid].state == ZOMBIE){
        kill(pid);
        return pid;
    }
    sch->procesess[pid].waitingForMe[sch->procesess[pid].waitingIndex] = getProcessCurrentPid();
    sch->procesess[pid].waitingIndex++;
    block(sch->currentPid);
    return pid;
}

void unblockWaiters(uint8_t pid)
{
    char found = 0;
    uint8_t waitingInex = sch->procesess[pid].waitingIndex;
    for (int i = 0; i < waitingInex; ++i) {
        PCB * proc = sch->procesess[pid].waitingForMe[i];
        if(proc != NULL && proc->state != NOT_EXISTS)
        {
            proc->state = READY;
            found = 1;
        }
    }
    if(found){
        kill(pid);
    }
}





/**
*   Dentro del PCB --> Guardamos SP y estado (ready, bloqueado)
*   Debo iterar por una lista de PCB y a los ready los corro y a los que no los salteo
*
*/