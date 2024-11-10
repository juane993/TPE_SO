// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdint.h>
#include <stdio.h>
#define BLOCK_COUNT 128
#define START_FREE_MEM 600000

uint64_t test_mm(uint64_t argc, char *argv[]);

void *start;
int block_size;

void *free_ptrs[BLOCK_COUNT];
int current;

void createMemoryManager(void *p, int size) {
	start = p;
	block_size = size;
	for (int i = 0; i < BLOCK_COUNT; i++) {
		free_ptrs[i] = (char *) start + i * block_size;
	}
	current = 0;
}

void *allocMemory(uint32_t size) {
	if (size > block_size) {
		return NULL;
	}
	if (current < BLOCK_COUNT) {
		return free_ptrs[current++];
	}
	return NULL;
}

void my_free(void *p) {
	free_ptrs[--current] = p;
}

int main(int argc, char *argv[]) {
	int num = test_mm(argc - 1, argv);
}
