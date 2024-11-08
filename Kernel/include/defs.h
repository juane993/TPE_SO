/***************************************************
  Defs.h
****************************************************/

#ifndef _DEFS_H_
#define _DEFS_H_


/* Flags para derechos de acceso de los segmentos */
#define ACS_PRESENT     0x80            /* segmento presente en memoria */
#define ACS_CSEG        0x18            /* segmento de codigo */
#define ACS_DSEG        0x10            /* segmento de datos */
#define ACS_READ        0x02            /* segmento de lectura */
#define ACS_WRITE       0x02            /* segmento de escritura */
#define ACS_IDT         ACS_DSEG
#define ACS_INT_386 	0x0E		/* Interrupt GATE 32 bits */
#define ACS_INT         ( ACS_PRESENT | ACS_INT_386 )

#define MEM_BLOCK_SIZE 2048
#define LOGIC_DOWN (void *)0x5

#define NULL ((void *)0)
#define EOF (-1)

#define FALSE 0
#define TRUE 1
#define EMPTY_FD -1

/* File Descriptors*/
#define STDIN 0
#define STDOUT 1
#define STDERR 2
#define KBDIN 3

#define MAX_PIPES 30
#define MAX_FDS 10

typedef enum {
    LOW = 1,
    MEDIUM = 5,
    HIGH = 10,
    SHELL = 20
} Priority;

typedef enum {
    READY,
    BLOCK,
    RUNNING,
    NOT_EXISTS,
    ZOMBIE
} State;

typedef enum {
    BACKGROUND = 0,
    FOREGROUND = 1
} BackgroundState;

#define ACS_CODE        (ACS_PRESENT | ACS_CSEG | ACS_READ)
#define ACS_DATA        (ACS_PRESENT | ACS_DSEG | ACS_WRITE)
#define ACS_STACK       (ACS_PRESENT | ACS_DSEG | ACS_WRITE)


#endif