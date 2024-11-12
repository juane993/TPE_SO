// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// test_mm.c
#include <syscalls.h>
#include <test_mm.h>
#include <test_util.h>
#include <stdio.h>
#include <stdlib.h>

void createMemoryManager(void * p, int s);

#define MAX_BLOCKS 50

typedef struct MM_rq
{
    void * address;
    uint32_t size;
} mm_rq;

uint64_t test_mm(int argc, char * argv[])
{
    uint64_t maxmem = atoi(argv[0]);
    mm_rq mm_rqs[MAX_BLOCKS];
    uint8_t rq;
    uint32_t total;
    uint64_t max_memory;

    max_memory = maxmem;

    int z = 0;
    while (z < 20)
    {
        rq          = 0;
        total       = 0;
        char finish = 0;
        // Request as many blocks as we can
        while (rq < MAX_BLOCKS && total < max_memory && !finish)
        {

            //			mm_rqs[rq].size = GetUniform(max_memory - total - 1) +
            // 1;
            mm_rqs[rq].size = GetUniform(32);

            mm_rqs[rq].address = allocMemory(mm_rqs[rq].size);

            if (mm_rqs[rq].address)
            {
                total += mm_rqs[rq].size;
                rq++;
            }
            else
            {
                finish = 1;
            }
        }

        // Set
        uint32_t i;
        for (i = 0; i < rq; i++)
        {
            if (mm_rqs[i].address)
            {
                // Mostrar información antes de memset
                // printf("Bloque %d: Dirección: %p, Tamaño: %u bytes\n", i,
                // mm_rqs[i].address, mm_rqs[i].size); memset(mm_rqs[i].address,
                // i, mm_rqs[i].size); // fuera de nuestro kernel anda

                // memset de reemplazo para nuestro kernel
                // char * temp = (char *) mm_rqs[i].address;
                memset(mm_rqs[i].address, i, mm_rqs[i].size);
                //                for (int j = 0; j < mm_rqs[i].size; ++j) {
                //                    ((uint8_t *)mm_rqs[i].address)[j] = i;
                //                }
            }
        }

        // Mostrar el contenido de los bloques después de memset
        for (i = 0; i < rq; i++)
        {
            if (mm_rqs[i].address)
            {
                printf("Contenido del bloque %d en direccion %x: ", i, mm_rqs[i].address);
                for (uint32_t j = 0; j < mm_rqs[i].size; j++)
                {
                    printf("%d ", ((uint8_t *) mm_rqs[i].address)[j]);
                }
                printf("\n");
            }
        }

        // Check
        for (i = 0; i < rq; i++)
        {
            if (mm_rqs[i].address)
            {
                if (!memcheck(mm_rqs[i].address, i, mm_rqs[i].size))
                {
                    printf("test_mm ERROR en bloque: %d: ", i);
                    for (uint32_t j = 0; j < mm_rqs[i].size; j++)
                    {
                        printf("%d ", ((uint8_t *) mm_rqs[i].address)[j]);
                    }
                    printf("\n");
                    return -1;
                }
            }
        }

        //	 Free
        for (i = 0; i < rq; i++)
        {
            if (mm_rqs[i].address)
                my_free(mm_rqs[i].address);
        }

        printf("TODO BIEN %d,\n", z);
        z++;
    }
    return 0;
}