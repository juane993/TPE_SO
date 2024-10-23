
#include <semaphore.h>


sem_t * semaphores[MAX_SEMAPHORES];


void initSemaphores()
{
    for (uint8_t i = 0; i < MAX_SEMAPHORES; i++) {
        semaphores[i] = NULL;
    }
    sem_init(MUTEX,1);
}


int8_t sem_init(uint8_t semIndex, uint8_t value)
{
    semaphores[semIndex] = allocMemory(MAX_SEMAPHORES * sizeof(sem_t *));
    sem_t * sem = semaphores[semIndex];
    if(sem == NULL)
    {
        return -1;
    }
    sem->value = value;
    sem->lock = value?1:0;
    sem->waitingIndex = 0;
    sem->postingIndex = 0;
//    sem->waiting = allocMemory(MAX_PROCESSES * sizeof(PCB *));
    for (uint8_t i = 0; i < MAX_PROCESSES; i++) {
        sem->waiting[i] = NULL;
    }
    return 0;
}

int8_t sem_free(uint8_t semIndex)
{
    sem_t * sem = semaphores[semIndex];
    if(sem == NULL)
    {
        return -1;
    }
    my_free(sem->waiting);
    my_free(sem);
    semaphores[semIndex] = NULL;
    return 0;
}

void sem_wait(uint8_t semIndex)
{
    sem_t * sem = semaphores[semIndex];
    if(sem == NULL)
    {
        return;
    }
    acquire(&(semaphores[MUTEX]->lock));

    switch (sem->value) {

        case 0:
//            sem->waiting[sem->waitingIndex] = &(sch->procesess[sch->currentPid]);
            sem->waiting[sem->waitingIndex] = getProcessCurrentPid();
            if(sem->waitingIndex == MAX_PROCESSES - 1)
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
    release(&(semaphores[MUTEX]->lock));
}

void sem_post(uint8_t semIndex)
{
    sem_t * sem = semaphores[semIndex];
    if (sem == NULL) {
        return;
    }
    acquire(&(semaphores[MUTEX]->lock));
    switch (sem->value) {
        case 0:
            if(sem->waitingIndex == sem->postingIndex)
            {
                sem->value++;
            }
            else{
                unblock(sem->waiting[sem->postingIndex]->processPid);
                sem->waiting[sem->postingIndex] = NULL;
                if(sem->postingIndex == MAX_PROCESSES - 1)
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
    release(&(semaphores[MUTEX]->lock));
}