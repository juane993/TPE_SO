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

#define MEM_BLOCK_SIZE 1000
#define LOGIC_DOWN (void *)0x5

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
    NOT_EXISTS
} State;

#define ACS_CODE        (ACS_PRESENT | ACS_CSEG | ACS_READ)
#define ACS_DATA        (ACS_PRESENT | ACS_DSEG | ACS_WRITE)
#define ACS_STACK       (ACS_PRESENT | ACS_DSEG | ACS_WRITE)


#endif