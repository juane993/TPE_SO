// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// test_mm.c
#include "syscall.h"
#include "test_util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void *allocMemory(uint32_t size);
void my_free(void *p);
void createMemoryManager(void *p, int s);

#define MAX_BLOCKS 128

typedef struct MM_rq {
	void *address;
	uint32_t size;
} mm_rq;

uint64_t test_mm(uint64_t argc, char *argv[]) {
	char mem[MAX_BLOCKS * 20];
	createMemoryManager((void *) mem, 20);
	mm_rq mm_rqs[MAX_BLOCKS];
	uint8_t rq;
	uint32_t total;
	uint64_t max_memory;

	if (argc != 1) {
		printf("Estoy en test.");
		return -1;
	}
	if ((max_memory = satoi(argv[1])) <= 0)
		return -1;

	int z = 0;
	while (z < 130) {
		rq = 0;
		total = 0;

		// Request as many blocks as we can
		while (rq < MAX_BLOCKS && total < max_memory) {
			mm_rqs[rq].size = GetUniform(max_memory - total - 1) + 1;
			printf("%d\n", mm_rqs[rq].size);
			mm_rqs[rq].address = allocMemory(mm_rqs[rq].size);
			if (mm_rqs[rq].address) {
				total += mm_rqs[rq].size;
				rq++;
			}
		}

		// Set
		uint32_t i;
		for (i = 0; i < rq; i++) {
			if (mm_rqs[i].address) {
				// Mostrar información antes de memset
				printf("Bloque %d: Dirección: %p, Tamaño: %u bytes\n", i, mm_rqs[i].address, mm_rqs[i].size);
				memset(mm_rqs[i].address, i, mm_rqs[i].size);
			}
		}

		// Mostrar el contenido de los bloques después de memset
		for (i = 0; i < rq; i++) {
			if (mm_rqs[i].address) {
				printf("Contenido del bloque %d en dirección %p: ", i, mm_rqs[i].address);
				for (uint32_t j = 0; j < mm_rqs[i].size; j++) {
					printf("%u ", ((uint8_t *) mm_rqs[i].address)[j]);
				}
				printf("\n");
			}
		}

		// Check
		for (i = 0; i < rq; i++) {
			if (mm_rqs[i].address) {
				if (!memcheck(mm_rqs[i].address, i, mm_rqs[i].size)) {
					printf("test_mm ERROR\n");
					return -1;
				}
			}
		}

		//	 Free
		for (i = 0; i < rq; i++) {
			if (mm_rqs[i].address)
				my_free(mm_rqs[i].address);
		}

		printf("TODO BIEN %d,\n", z);
		z++;
	}
	return 0;
}