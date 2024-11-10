#include <process.h>
#include <keyboard.h>
#include <pipes.h>
#include <semaphore.h>
#include <video.h>

// OJO, quiero ver si tengo una race condition y por eso explota
//despues debe haber mejor manera de hacerlo pero quiero que no me interrumpan
//cuando estoy cerrando la wrapper
extern void disableInterrupts();
extern void enableInterrupts();



uint64_t initProcess(PCB *process, void *mainFunc, uint8_t argc, char **args, uint8_t newProcessPid, Priority prio, BackgroundState bgState, uint8_t parentPid) {
    uint64_t *stackBase = allocMemory(MEM_BLOCK_SIZE);
    if (stackBase == NULL) {
        return -1;
    }
    process->argc = argc;
    process->argv = allocMemory(MEM_BLOCK_SIZE);
    if (process->argv == NULL) {
        my_free(stackBase);
        return -1;
    }
    for (int i = 0; i < argc; ++i) {
        process->argv[i] = allocMemory(MEM_BLOCK_SIZE);
        if(process->argv[i] == NULL){
            for (int j = 0; j < i; ++j) {
                my_free(process->argv[j]);
            }
            my_free(process->argv);
            my_free(stackBase);
            return -1;
        }
        memcpy(process->argv[i], args[i], 20);
    }
    uint64_t *rsp = initProcessStack(processWrapper, stackBase, argc, process->argv, mainFunc);
    if (rsp == NULL) {
        for (int i = 0; i < argc; ++i) {
            my_free(process->argv[i]);
        }
        my_free(process->argv);
        my_free(stackBase);
        return -1;
    }

    process->priority = prio;
    process->state = READY;
    process->processPid = newProcessPid;
    process->RSP = (uint8_t *) rsp;
    process->stackBase = (uint8_t *) stackBase;
    process->parentPid = parentPid;
    for (int i = 0; i < 40; ++i) {
        process->waitingForMe[i].waitingPCB = NULL;
        process->children[i] = NULL;
    }

    //process->argv = args;
    process->bgState = bgState;
    process->waitingIndex = 0;
    process->waitingForFD = EMPTY_FD;
    process->openFDs[0] = STDIN;
    process->openFDs[1] = STDOUT;
    process->openFDs[2] = STDERR;
    process->openFDs[3] = KBDIN;
    for (int i = 4; i < MAX_FDS; ++i) {
        process->openFDs[i] = EMPTY_FD;
    }
    //process->waitingForMe[0] = getProcess(parentPid);

    return process->processPid;
}

uint32_t killProcess(PCB *process) {
    if (process->state == NOT_EXISTS) {
        return -1;
    }
    for (int i = 0; i < process->argc; ++i) {
        my_free(process->argv[i]);
    }
    my_free(process->argv);
    my_free(process->stackBase);
    process->state = NOT_EXISTS;
    return process->processPid;
}
void closePipeEnds()
{
    PCB * proc = getProcessCurrentPid();
    int i = 0;
    while (i < MAX_FDS)
    {
        int8_t fd = proc->openFDs[i];
        if(fd != EMPTY_FD && fd > KBDIN)
        {
            if(fd % 2 == 1)
            {
                char eof = EOF;
                writePipe(fd, &eof, 1);
            }
            closePipeEnd(fd);
        }
        i++;
    }
}

void my_exit()
{
    sem_wait(MUTEX);
    PCB * proc = getProcessCurrentPid();
    closePipeEnds();
    adoptOrphans(getCurrentPid());
    unblockWaiters(getCurrentPid());
    zombie(getCurrentPid());
    sem_post(MUTEX);
}

void processWrapper(char argc, char *argv[], void *mainFunc) {

    uint64_t ret = ((int (*)(int, char **))mainFunc)(argc, argv);
    //disableInterrupts();
    my_exit();
    //enableInterrupts();
    yield();
}




uint8_t readStdin(){
    char c = getAscii();
    if(c == 0)
    {

        block(getCurrentPid());
        PCB * proc = getProcessCurrentPid();
        proc->waitingForFD = STDIN;
        yield();
        c = getAscii();
    }
    return c;
}

uint8_t mapFD(uint8_t pid, uint8_t oldFD, uint8_t newFD)
{
    PCB * proc = getProcess(pid);
    if(oldFD < 0 || oldFD >= MAX_FDS || newFD < 0 || newFD >= MAX_FDS)
    {
        return -1;
    }
    proc->openFDs[oldFD] = newFD;

    return 0;
}

uint8_t transformFD(uint8_t fd){
    return getProcessCurrentPid()->openFDs[fd];
}

uint8_t read(int8_t fd, char *buf, uint8_t size) {
    fd = transformFD(fd);
    switch (fd){
        case STDIN:
            *buf = readStdin();
            return 1;
        case KBDIN:
            *buf = getScancode();
            return 1;
        default:
            return readPipe(fd,buf,size);
    }
}
uint8_t write(int8_t fd, char *buf, uint8_t size)
{
    fd = transformFD(fd);
    Color prevColor = getFontColor();
    if(fd == STDERR)
    {
        setFontColor(ERROR_COLOR);
    }
    else if(fd > KBDIN)
    {
        return writePipe(fd, buf, size);
    }
    printChar(buf[0]);
    setFontColor(prevColor);
    return 1;
}

uint32_t getSeconds() {
    uint8_t h, m, s;
    getTime(&h, &m, &s);
    return s + m * 60 + ((h + 24 - 3) % 24) * 3600;
}

void sleep(int8_t sec){
    uint32_t limit = getSeconds() + sec;
    while (getSeconds() < limit)
        yield();
}