// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdint.h>
#include <video.h>
#include <keyboard.h>
#include <lib.h>
#include <color.h>
#include <speaker.h>
#include <time.h>
#include <memory.h>
#include <MemoryManager.h>
#include <scheduler.h>
#include <semaphore.h>
#include <pipes.h>
#include <process.h>
#include <defs.h>

/* IDs de syscalls */
#define READ               0
#define WRITE              1
#define CLEAR              2
#define SECONDS            3
#define GET_REGISTER_ARRAY 4
#define SET_FONT_SIZE      5
#define GET_RESOLUTION     6
#define DRAW_RECT          7
#define GET_TICKS          8
#define GET_MEMORY         9
#define PLAY_SOUND         10
#define SET_FONT_COLOR     11
#define GET_FONT_COLOR     12

#define MALLOC         13
#define FREE           14
#define CREATE_PROCESS 15
#define KILL_PROC      16
#define BLOCK_PROC     17
#define UNBLOCK_PROC   18
#define NICE_PROC      19
#define EXIT           20
#define YIELD          21
#define PS             22
#define GET_PID        23
#define SEMWAIT        24
#define SEMPOST        25
#define SEMINIT        26
#define SEMFREE        27
#define WAITPID        28
#define WAITPIDNOHANG  29
#define MAPFD          30
#define OPENPIPE       31
#define CLOSEPIPE      32
#define SLEEP          33
#define PRINTMEM       34

static uint8_t syscall_read(uint32_t fd, char * c, uint8_t size);
static uint8_t syscall_write(uint32_t fd, char * c, uint8_t size);
static void syscall_clear();
static uint32_t syscall_seconds();
static uint64_t * syscall_registerArray(uint64_t * regarr);
static void syscall_fontSize(uint8_t size);
static uint32_t syscall_resolution();
static void syscall_drawRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
static uint64_t syscall_getTicks();
static void syscall_getMemory(uint64_t pos, uint8_t * vec);
static void syscall_playSound(uint64_t frequency, uint64_t ticks);
static void syscall_setFontColor(uint8_t r, uint8_t g, uint8_t b);
static uint32_t syscall_getFontColor();
static void * syscall_malloc(uint64_t size);
static void syscall_free(void * ptr);
static int32_t syscall_create_process(char * name, int8_t argc, char * argv[], Priority prio, BackgroundState bgState);
static int32_t syscall_kill(int32_t pid);
static int32_t syscall_block(int32_t pid);
static int32_t syscall_unblock(int32_t pid);
static int32_t syscall_nice(int32_t pid, Priority prio);
static void syscall_exit();
static void syscall_yield();
static char * syscall_ps();
static int8_t syscall_getpid();

static void syscall_semWait(uint8_t id);
static void syscall_semPost(uint8_t id);
static int8_t syscall_semInit(uint8_t id, uint8_t value);
static void syscall_semFree(uint8_t id);
static int8_t syscall_waitPid(uint8_t pid);

static int8_t syscall_waitPidNoHang(uint8_t pid);

static int8_t syscall_mapFD(uint8_t pid, uint8_t oldFD, uint8_t newFD);
static int8_t syscall_openPipe(int8_t * fd);
static int8_t syscall_closePipeEnd(int8_t fd);

static void syscall_sleep(int8_t sec);

static char * syscall_printMem();

int64_t syscallDispatcher(uint64_t nr, uint64_t arg0, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5)
{
    switch (nr)
    {
    case READ:
        return syscall_read((uint32_t) arg0, (char *) arg1, (uint8_t) arg2);
    case WRITE:
        syscall_write((uint32_t) arg0, (char *) arg1, (uint8_t) arg2);
        break;
    case CLEAR:
        syscall_clear();
        break;
    case SECONDS:
        return syscall_seconds();
    case GET_REGISTER_ARRAY:
        return (uint64_t) syscall_registerArray((uint64_t *) arg0);
    case SET_FONT_SIZE:
        syscall_fontSize((uint8_t) arg0);
        break;
    case GET_RESOLUTION:
        return syscall_resolution();
    case DRAW_RECT:
        syscall_drawRect((uint16_t) arg0, (uint16_t) arg1, (uint16_t) arg2, (uint16_t) arg3, (uint32_t) arg4);
        break;
    case GET_TICKS:
        return syscall_getTicks();
    case GET_MEMORY:
        syscall_getMemory((uint64_t) arg0, (uint8_t *) arg1);
        break;
    case PLAY_SOUND:
        syscall_playSound(arg0, arg1);
        break;
    case SET_FONT_COLOR:
        syscall_setFontColor((uint8_t) arg0, (uint8_t) arg1, (uint8_t) arg2);
        break;
    case GET_FONT_COLOR:
        return syscall_getFontColor();
    case MALLOC:
        return (uint64_t) syscall_malloc((uint64_t) arg0);
    case FREE:
        syscall_free((void *) arg0);
        break;
    case CREATE_PROCESS:
        return syscall_create_process((char *) arg0, (int8_t) arg1, (char **) arg2, (Priority) arg3, (BackgroundState) arg4);
    case KILL_PROC:
        return syscall_kill((int32_t) arg0);
    case BLOCK_PROC:
        return syscall_block((int32_t) arg0);
    case UNBLOCK_PROC:
        return syscall_unblock((int32_t) arg0);
    case NICE_PROC:
        return syscall_nice((int32_t) arg0, (Priority) arg2);
    case EXIT:
        syscall_exit();
        break;
    case YIELD:
        syscall_yield();
        break;
    case PS:
        return (int64_t) syscall_ps();
    case GET_PID:
        return syscall_getpid();
    case SEMWAIT:
        syscall_semWait((uint8_t) arg0);
        break;
    case SEMPOST:
        syscall_semPost((uint8_t) arg0);
        break;
    case SEMINIT:
        return syscall_semInit((uint8_t) arg0, (uint8_t) arg1);
    case SEMFREE:
        syscall_semFree((uint8_t) arg0);
        break;
    case WAITPID:
        return syscall_waitPid((uint8_t) arg0);
    case WAITPIDNOHANG:
        return syscall_waitPidNoHang((uint8_t) arg0);
    case MAPFD:
        return syscall_mapFD((uint8_t) arg0, (uint8_t) arg1, (uint8_t) arg2);
    case OPENPIPE:
        return syscall_openPipe((int8_t *) arg0);
    case CLOSEPIPE:
        return syscall_closePipeEnd((int8_t) arg0);
    case SLEEP:
        syscall_sleep((int8_t) arg0);
        break;
    case PRINTMEM:
        return (char *) syscall_printMem();
    }
    return 0;
}

// Read char
static uint8_t syscall_read(uint32_t fd, char * c, uint8_t size)
{
    return read(fd, c, size);
}

// Write
static uint8_t syscall_write(uint32_t fd, char * buf, uint8_t size)
{

    return write(fd, buf, size);
}

// Clear
static void syscall_clear()
{
    videoClear();
}

// Get time in seconds
static uint32_t syscall_seconds()
{
    uint8_t h, m, s;
    getTime(&h, &m, &s);
    return s + m * 60 + ((h + 24 - 3) % 24) * 3600;
}

// Get register snapshot array
static uint64_t * syscall_registerArray(uint64_t * regarr)
{
    uint64_t * snapshot = getLastRegSnapshot();
    for (int i = 0; i < QTY_REGS; i++)
        regarr[i] = snapshot[i];
    return regarr;
}

// Set fontsize
static void syscall_fontSize(uint8_t size)
{
    setFontSize(size - 1);
}

// Get screen resolution
static uint32_t syscall_resolution()
{
    return getScreenResolution();
}

// DrawRect
static void syscall_drawRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color)
{
    ColorInt myColor = {bits : color};
    drawRect(x, y, width, height, myColor.color);
}

// GetTicks
static uint64_t syscall_getTicks()
{
    return ticksElapsed();
}

// PrintMem
static void syscall_getMemory(uint64_t pos, uint8_t * vec)
{
    memcpy(vec, (uint8_t *) pos, 32);
}

// playSound
static void syscall_playSound(uint64_t frequency, uint64_t ticks)
{
    playSound(frequency, ticks);
}

// Set fontsize
static void syscall_setFontColor(uint8_t r, uint8_t g, uint8_t b)
{
    setFontColor((Color){b, g, r});
}

// Get fontsize
static uint32_t syscall_getFontColor()
{
    ColorInt c = {color : getFontColor()};
    return c.bits;
}

// Malloc i
static void * syscall_malloc(uint64_t size)
{
    return allocMemory(size);
}

// Free
static void syscall_free(void * ptr)
{
    my_free(ptr);
}

static int32_t syscall_create_process(char * name, int8_t argc, char * argv[], Priority prio, BackgroundState bgState)
{
    return createProcess(name, argc, argv, prio, bgState);
}

static int32_t syscall_kill(int32_t pid)
{
    return kill(pid);
}
static int32_t syscall_block(int32_t pid)
{
    return block(pid);
}
static int32_t syscall_unblock(int32_t pid)
{
    return unblock(pid);
}

static int32_t syscall_nice(int32_t pid, Priority prio)
{
    return nice(pid, prio);
}

static void syscall_exit()
{
    my_exit();
}

static void syscall_yield()
{
    yield();
}

static char * syscall_ps()
{
    return ps();
}

static int8_t syscall_getpid()
{
    return my_get_pid();
}

static void syscall_semWait(uint8_t id)
{
    sem_wait(id);
}

static void syscall_semPost(uint8_t id)
{
    sem_post(id);
}

static int8_t syscall_semInit(uint8_t id, uint8_t value)
{
    return sem_init(id, value);
}

static void syscall_semFree(uint8_t id)
{
    sem_free(id);
}

static int8_t syscall_waitPid(uint8_t pid)
{
    return waitPid(pid);
}

static int8_t syscall_waitPidNoHang(uint8_t pid)
{
    return waitPidNoHang(pid);
}

static int8_t syscall_mapFD(uint8_t pid, uint8_t oldFD, uint8_t newFD)
{
    return mapFD(pid, oldFD, newFD);
}
static int8_t syscall_openPipe(int8_t * fd)
{
    return initPipe(fd);
}
static int8_t syscall_closePipeEnd(int8_t fd)
{
    return closePipeEnd(fd);
}
static void syscall_sleep(int8_t sec)
{
    sleep(sec);
}

static char * syscall_printMem()
{
    return printMem();
}