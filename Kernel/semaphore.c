

#include <semaphore.h>

typedef struct semaphoreCDT
{
    sem_t semaphores[MAX_SEMAPHORES];
} semaphoreCDT;

semaphoreADT semManager;

void initSemaphores()
{
    semManager = (semaphoreADT) SEMPAHORE_MANAGER_ADDRESS;
    for (uint8_t i = 0; i < MAX_SEMAPHORES; i++)
    {
        // semManager->semaphores[i] = NULL;
        semManager->semaphores[i].id = NOT_USED;
    }
    sem_init(MUTEX, 1);
}

int8_t sem_init(uint8_t semIndex, uint8_t value)
{
    // semManager->semaphores[semIndex] = allocMemory( sizeof(sem_t *));
    sem_t * sem = &(semManager->semaphores[semIndex]);
    if (semIndex >= MAX_SEMAPHORES || sem->id != NOT_USED)
    {
        return -1;
    }
    sem->id           = semIndex;
    sem->value        = value;
    sem->lock         = value ? 1 : 0;
    sem->waitingIndex = 0;
    sem->postingIndex = 0;
    //    sem->waiting = allocMemory(MAX_PROCESSES * sizeof(PCB *));
    for (uint8_t i = 0; i < MAX_PROCESSES; i++)
    {
        sem->waiting[i] = NULL;
    }
    return 0;
}

int8_t sem_free(uint8_t semIndex)
{
    sem_t * sem = &(semManager->semaphores[semIndex]);
    if (semIndex < 0 || semIndex >= MAX_SEMAPHORES || sem->id == NOT_USED)
    {
        return -1;
    }
    // my_free(sem->waiting);
    // my_free(sem);
    sem->id = NOT_USED;
    // semManager->semaphores[semIndex] = NULL;
    return 0;
}

void sem_wait(uint8_t semIndex)
{
    sem_t * sem = &(semManager->semaphores[semIndex]);
    if (semIndex < 0 || semIndex >= MAX_SEMAPHORES || sem->id == NOT_USED)
    {
        return;
    }
    acquire(&(semManager->semaphores[MUTEX].lock));

    switch (sem->value)
    {

    case 0:
        //            sem->waiting[sem->waitingIndex] =
        //            &(sch->procesess[sch->currentPid]);
        sem->waiting[sem->waitingIndex] = getProcessCurrentPid();
        if (sem->waitingIndex == MAX_PROCESSES - 1)
        {
            sem->waitingIndex = 0;
        }
        else
        {
            sem->waitingIndex++;
        }
        block(getCurrentPid());
        break;
    default:
        sem->value--;
        break;
    }
    release(&(semManager->semaphores[MUTEX].lock));
}

void sem_post(uint8_t semIndex)
{
    sem_t * sem = &(semManager->semaphores[semIndex]);
    if (semIndex < 0 || semIndex >= MAX_SEMAPHORES || sem->id == NOT_USED)
    {
        return;
    }
    acquire(&(semManager->semaphores[MUTEX].lock));
    switch (sem->value)
    {
    case 0:
        if (sem->waitingIndex == sem->postingIndex)
        {
            sem->value++;
        }
        else
        {
            unblock(sem->waiting[sem->postingIndex]->processPid);
            sem->waiting[sem->postingIndex] = NULL;
            if (sem->postingIndex == MAX_PROCESSES - 1)
            {
                sem->postingIndex = 0;
            }
            else
            {
                sem->postingIndex++;
            }
        }
        break;
    default:
        sem->value++;
        break;
    }
    release(&(semManager->semaphores[MUTEX].lock));
}