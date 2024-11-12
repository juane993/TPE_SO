// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <philo.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <syscalls.h>
#define MAX_QTY                 10
#define MIN_QTY                 3
#define BASE_QTY                5
#define MUTEX_SEM_ID            8 // 12 -> 6 muere, 13 -> 5 muere
#define MAX_PHILO_NUMBER_BUFFER 3
#define EAT_TIME                3
#define THINK_TIME              2
#define COMMAND_QUIT            'q'
#define COMMAND_ADD             'a'
#define COMMAND_REMOVE          'r'
#define left(i)                 (((i) + qtyPhilosophers - 1) % qtyPhilosophers)
#define right(i)                (((i) + 1) % qtyPhilosophers)
#define philosopherSemaphore(i) (MUTEX_SEM_ID + (i) + 1)
typedef enum
{
    NONE = 0,
    EATING,
    HUNGRY,
    THINKING
} PHILOSOPHER_STATE;

static uint8_t qtyPhilosophers = 0;
static PHILOSOPHER_STATE philosopherStates[MAX_QTY];
static int16_t philosopherPids[MAX_QTY];

static void render();
static int philos(int argc, char ** argv);
static int8_t addPhilosopher(int index);
static int8_t removePhilosopher(int index);

static void takeForks(int i);
static void putForks(int i);
static void test(int i);

int run_philosophers()
{
    if (my_semInit(MUTEX_SEM_ID, 1) == -1)
        return -1;
    for (int i = 0; i < MAX_QTY; i++)
    {
        philosopherStates[i] = NONE;
        philosopherPids[i]   = -1;
    }
    for (int i = 0; i < BASE_QTY; i++)
        addPhilosopher(i);
    char command = '\0';

    while ((command = getchar()) != COMMAND_QUIT)
    {
        switch (command)
        {
        case COMMAND_ADD:
            if (qtyPhilosophers < MAX_QTY)
            {
                if (addPhilosopher(qtyPhilosophers) == -1)
                    printf("Error en addPhilosopher, habia espacio, probablemte en MUTEX_SEM_ID\n");
            }
            else
            {
                printf("La mesa esta llena \n");
            }
            break;
        case COMMAND_REMOVE:
            if (qtyPhilosophers > MIN_QTY)
            {
                removePhilosopher(qtyPhilosophers - 1);
            }
            break;
        }
    }
    for (int i = qtyPhilosophers - 1; i >= 0; i--)
    {
        removePhilosopher(i);
    }

    my_semClose(MUTEX_SEM_ID);
    printf("VOLVEEEEE A LA TERMINAL");
    return 0;
}
static void render()
{
    const static char letters[] = {' ', 'E', '_', '.'};
    uint8_t somethingToWrite    = 0;
    for (int i = 0; i < qtyPhilosophers; i++)
    {
        if (letters[philosopherStates[i]] != ' ')
        {
            somethingToWrite = 1;
            printf("%c   ", letters[philosopherStates[i]]);
        }
    }
    if (somethingToWrite)
        putchar('\n');
    //    my_sleep(1);  // Pueden sacarlo, se agruega para que se lea mejor
}
static int8_t addPhilosopher(int index)
{
    my_semWait(MUTEX_SEM_ID);
    char philoNumberBuffer[MAX_PHILO_NUMBER_BUFFER] = {0};
    if (my_semInit(philosopherSemaphore(index), 0) == -1)
        return -1;
    itoa(index, philoNumberBuffer, 10);
    char * params[]        = {"philos", philoNumberBuffer, NULL};
    philosopherPids[index] = my_create_process(&philos, 2, params, HIGH, 1);
    if (philosopherPids[index] != -1)
        qtyPhilosophers++;
    render();
    my_semPost(MUTEX_SEM_ID);
    return (philosopherPids[index] == -1) ? -1 : 0;
}
static int8_t removePhilosopher(int index)
{
    my_semWait(MUTEX_SEM_ID);
    my_kill(philosopherPids[index]);
    my_waitPid(philosopherPids[index]);
    printf("se retiro el filosofo %d\n", index);
    my_semClose(philosopherSemaphore(index));
    philosopherPids[index]   = -1;
    philosopherStates[index] = NONE;
    qtyPhilosophers--;
    render();
    my_semPost(MUTEX_SEM_ID);
    return 0;
}
static int philos(int argc, char ** argv)
{
    int i = atoi(argv[1]);
    printf("entra el filosofo %d\n", i);
    philosopherStates[i] = THINKING;
    while (1)
    {
        my_sleep(THINK_TIME);
        takeForks(i);
        my_sleep(EAT_TIME);
        putForks(i);
    }
    return 0;
}

static void putForks(int i)
{
    my_semWait(MUTEX_SEM_ID);
    philosopherStates[i] = THINKING;
    render();
    test(left(i));
    test(right(i));
    my_semPost(MUTEX_SEM_ID);
}

static void takeForks(int i)
{
    my_semWait(MUTEX_SEM_ID);
    philosopherStates[i] = HUNGRY;
    test(i);
    render();
    my_semPost(MUTEX_SEM_ID);
    my_semWait(philosopherSemaphore(i));
}

static void test(int i)
{
    if (philosopherStates[i] == HUNGRY && philosopherStates[left(i)] != EATING && philosopherStates[right(i)] != EATING)
    {
        philosopherStates[i] = EATING;
        my_semPost(philosopherSemaphore(i));
        render();
    }
}