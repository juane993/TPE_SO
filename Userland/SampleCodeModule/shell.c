#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <shell.h>
#include <stdint.h>
#include <syscalls.h>
#include <tron.h>
#include <man.h>
#include <libasm.h>
#include <test_mm.h>
#include <test_processes.h>
#include <test_prio.h>
#include <process.h>
#include <inputParser.h>
#include <philo.h>

void waitForChildren(uint8_t pid);

/* Enum para la cantidad de argumentos recibidos */
typedef enum {NO_PARAMS = 0, SINGLE_PARAM, DUAL_PARAM, THREE_PARAM, FOUR_PARAM} functionType;
#define QTY_BYTES 32 /* Cantidad de bytes de respuesta del printmem */
#define DEFAULT_FONT_SIZE 1
#define MIN_FONT_SIZE 1
#define MAX_FONT_SIZE 3

#define WELCOME "Bienvenido a Cactiland OS!\n"
#define INVALID_COMMAND "Comando invalido!\n"
#define WRONG_PARAMS "La cantidad de parametros ingresada es invalida\n"
#define INVALID_FONT_SIZE "Dimension invalida de fuente\n"
#define CHECK_MAN "Escriba \"man %s\" para ver como funciona el comando\n"
#define CHECK_MAN_FONT "Escriba \"man font-size\" para ver las dimensiones validas\n"
#define EOF (-1)

typedef struct {
    char * name;                    // Nombre del comando
    char * description;             // Descripcion del comando (para help)
    union {                         // Puntero a la funcion
       int (*f)(void);
       int (*g)(char *);
       int (*h)(char *, char *);
       int (*i)(char *, char *, char *);
       int (*j)(char *, char *, char *, char *);
    };
    functionType ftype;             // Cantidad de argumentos del comando
} Command;

static int8_t help();
static int8_t man(char * command);
static int8_t printInfoReg();
static int8_t time();
static int8_t div(char * num, char * div);
static int8_t tron();
static int8_t tronZen();
static int8_t fontSize(char * size);
static int8_t printMem(char * pos);
static int8_t getCommandIndex(char * command);
static int8_t sys_ps();
static int8_t clearConsole();



static Command commands[QTY_COMMANDS];

void init() {
    initProcesess();
    commands[0] = (Command){"help", "Listado de comandos", .f = (void*) &help, NO_PARAMS};
    commands[1] = (Command){ "man", "Manual de uso de los comandos", .g = (void*) &man, SINGLE_PARAM};
    commands[2] = (Command){"inforeg", "Informacion de los registos que fueron capturados en un momento arbitrario de ejecucion del sistema", .f = (void*)&printInfoReg, NO_PARAMS};
    commands[3] = (Command){"time", "Despliega la hora actual UTC - 3", .f = (void*) &time, NO_PARAMS};
    commands[4] = (Command){ "div", "Hace la division entera de dos numeros naturales enviados por parametro", .h = (void*) &div, DUAL_PARAM};
    commands[5] = (Command){ "kaboom", "Ejecuta una excepcion de Invalid Opcode", .f = (void*) &kaboom, NO_PARAMS};
    commands[6] = (Command){ "tron", "Juego Tron Light Cycles", .f = (void*) &tron, NO_PARAMS};
    commands[7] = (Command){ "tron-zen", "Juego Tron Light Cycles con un unico jugador", .f = (void*) &tronZen, NO_PARAMS};
    commands[8] = (Command){ "font-size", "Cambio de dimensiones de la fuente. Para hacerlo escribir el comando seguido de un numero", .g = (void*) &fontSize, SINGLE_PARAM};
    commands[9] = (Command){ "printmem", "Realiza un vuelco de memoria de los 32 bytes posteriores a una direccion de memoria en formato hexadecimal enviada por parametro", .g = (void*) &printMem, SINGLE_PARAM};
    commands[10] = (Command){ "clear", "Limpia toda la pantalla", .f = (void*) &clearConsole, NO_PARAMS};
    commands[11] = (Command){ "testmem", "mem test", .h = (void*) &run_test_mem, DUAL_PARAM};
    commands[12] = (Command){ "testprocess", "test processes", .h = (void*) &run_test_proc, DUAL_PARAM};
    commands[13] = (Command){ "testprio", "test priorities", .f = (void*) &run_test_prio, SINGLE_PARAM};
    commands[14] = (Command){ "kill", "kill a process with pid ", .g = (void*) &kill, SINGLE_PARAM};
    commands[15] = (Command){ "ps", "program status ", .f = (void*) &sys_ps, NO_PARAMS};
    commands[16] = (Command){ "testsync", "test de sincronizacion", .j = (void*) &run_test_sync, FOUR_PARAM};
    commands[17] = (Command){"cat", "Imprime el STDIN tal como lo recibe", .g = (void*) &run_cat, SINGLE_PARAM};
    commands[18] = (Command){"wc", "Imprime la cantidad de lineas", .g = (void*) &run_wc, SINGLE_PARAM};
    commands[19] = (Command){"filter", "Imprime las vocales", .g = (void*) &run_filter, SINGLE_PARAM};
    commands[20] = (Command){"loop", "Imprime su ID con un saludo cada n segundos", .h = (void*) &run_loop, DUAL_PARAM};
    commands[21] = (Command){"philo", "Philosofer", .f = (void*) &run_philosophers, SINGLE_PARAM};
}

void createShellProc(){
    my_create_process(run_shell, 0, (void *)0, SHELL,FOREGROUND);
}



void shellWait(uint8_t pid, char ** args){
    BackgroundState bg;
    char found = 0;
    for (int i = 0; i < 4 && !found; ++i) {
        if (strcmp(args[i], "bg") == 0) {
            bg = BACKGROUND;
            found = 1;
        }
        else if(strcmp(args[i], "fg") == 0){
            bg = FOREGROUND;
            found = 1;
        }
    }
    if(!found){
        bg = FOREGROUND;
    }
    if(bg == FOREGROUND){
        my_waitPid(pid);
    } else{
        my_waitPidNoHang(pid);
    }
}

void run_shell(){
    init();
//    int index;
    int index1, index2;
    puts(WELCOME);
    while(1){
        putchar('>');
        char command[MAX_CHARS] = {0};
        char *prog1 = NULL;
        char *prog2 = NULL;
        scanfOneString(command);
        findPipe(command, &prog1, &prog2);

        char arg1[MAX_CHARS];
        char arg2[MAX_CHARS];
        char arg3[MAX_CHARS];
        char arg4[MAX_CHARS];

        char * args[] = {arg1, arg2, arg3, arg4};
        int argc = 4;
        int qtyParams = split(prog1, argc, args);

        index1 = getCommandIndex(prog1);
        if (index1 == -1) {
            if (prog1[0] != 0)
                printErr(INVALID_COMMAND);
            continue;
        }
        int funcParams = commands[index1].ftype;
        if(qtyParams != funcParams){
            printErr(WRONG_PARAMS);
            printf(CHECK_MAN, command);
            continue;
        }

        char prog2Arg1[MAX_CHARS];
        char prog2Arg2[MAX_CHARS];
        char prog2Arg3[MAX_CHARS];
        char prog2Arg4[MAX_CHARS];

        char * args2[] = {prog2Arg1, prog2Arg2, prog2Arg3, prog2Arg4};
        if ( prog2 != NULL){
            argc = 4;
            args2[0] = prog2Arg1;
            args2[1] = prog2Arg2;
            args2[2] = prog2Arg3;
            args2[3] = prog2Arg4;
            int qtyParams2 = split(prog2, argc, args2);


            index2 = getCommandIndex(prog2);
            if (index2 == -1) {
                if (prog2[0] != 0)
                    printErr(INVALID_COMMAND);
                continue;
            }
            int funcParams = commands[index2].ftype;
            if(qtyParams2 != funcParams){
                printErr(WRONG_PARAMS);
                printf(CHECK_MAN, prog2);
                continue;
            }
        }

        int pid1;
        switch (commands[index1].ftype)
        {
            case NO_PARAMS:
               pid1 = commands[index1].f();
                break;
            case SINGLE_PARAM:
               pid1 = commands[index1].g(arg1);
                break;
            case DUAL_PARAM:
                pid1 = commands[index1].h(arg1, arg2);
                break;
            case THREE_PARAM:
               pid1 =  commands[index1].i(arg1, arg2, arg3);
                break;
            case FOUR_PARAM:
               pid1 = commands[index1].j(arg1, arg2, arg3,arg4);
                break;
        }
         if (prog2 != NULL && pid1 != -1){
            int pid2;
            switch (commands[index2].ftype)
            {
                case NO_PARAMS:
                   pid2 = commands[index2].f();
                    break;
                case SINGLE_PARAM:
                   pid2 = commands[index2].g(prog2Arg1);
                    break;
                case DUAL_PARAM:
                   pid2 = commands[index2].h(prog2Arg1, prog2Arg2);
                    break;
                case THREE_PARAM:
                   pid2 = commands[index2].i(prog2Arg1, prog2Arg2, prog2Arg3);
                    break;
                case FOUR_PARAM:
                    pid2 = commands[index2].j(prog2Arg1, prog2Arg2, prog2Arg3,prog2Arg4);
                    break;
            }
            int8_t pipesFD[2];

            if(pid1 != -1 && pid2 != -1){
            my_openPipe(pipesFD);
            my_mapFD(pid1, STDOUT, pipesFD[1]);
            my_mapFD(pid2, STDIN, pipesFD[0]);
             shellWait(pid2,args2);
             my_waitPidNoHang(pid1);
            }
        }
         else if(pid1 != -1){
             shellWait(pid1,args);
         }

    }
}


/**
 * @brief  Devuelve el indice del vector de comandos dado su nombre
 * @param  command: Nombre del comando a buscar
 * @return  Indice del comando 
 */
static int8_t getCommandIndex(char * command) {
    int idx = 0;
    for(; idx < QTY_COMMANDS; idx++){
        if(!strcmp(commands[idx].name, command))
            return idx;
    }
    return -1;

}

static int8_t help() {
    for (int i = 0; i < QTY_COMMANDS; i++)
        printf("%s: %s\r\n", commands[i].name, commands[i].description);

    return -1;
}

static int8_t div(char * num, char * div) {
    printf("%s/%s=%d\r\n", num, div, atoi(num)/atoi(div));
    return -1;
}



static int8_t time(){
    uint32_t secs = getSeconds();
    uint32_t h = secs / 3600, m = secs % 3600 / 60, s = secs % 3600 % 60;
    printf("%2d:%2d:%2d\r\n", h, m, s);

    return -1;
}

static int8_t fontSize(char * size) {
    int s = atoi(size);
    if(s >= MIN_FONT_SIZE && s <= MAX_FONT_SIZE)
        setFontSize((uint8_t)atoi(size));
    else{
        printErr(INVALID_FONT_SIZE);
        puts(CHECK_MAN_FONT);
    }

    return -1;
}

static int8_t tron(){
    setFontSize(2);
    startTron(2);
    setFontSize(1);

    return -1;
}

static int8_t tronZen(){
    setFontSize(2);
    startTron(1);
    setFontSize(1);

    return -1;
}


static int8_t printMem(char * pos){
    uint8_t resp[QTY_BYTES];
    char * end;
    getMemory(strtoh(pos, &end), resp);
    for(int i = 0; i < QTY_BYTES; i++) {
        printf("0x%2x ", resp[i]);
        if (i % 4 == 3)
            putchar('\n');
    }

    return -1;
}

static char * _regNames[] = {"RIP", "RSP", "RAX", "RBX", "RCX", "RDX", "RBP", "RDI", "RSI", "R8", "R9", "R10", "R11", "R12", "R13", "R14", "R15"};
static int8_t printInfoReg() {
    int len = sizeof(_regNames)/sizeof(char *);
    uint64_t regSnapshot[len];
    getInfoReg(regSnapshot);
    for (int i = 0; i < len; i++)
        printf("%s: 0x%x\n", _regNames[i], regSnapshot[i]);

    return -1;
}

static int8_t man(char * command){
    int idx = getCommandIndex(command);
    if (idx != -1)
        printf("%s\n", usages[idx]);
    else
        printErr(INVALID_COMMAND);

    return -1;
}

static int8_t sys_ps(){
    char * ps = my_ps();
    printf("%s", ps);
    my_free((void *)ps);

    return -1;
}

static int8_t clearConsole(){
    clear();
    return -1;
}


