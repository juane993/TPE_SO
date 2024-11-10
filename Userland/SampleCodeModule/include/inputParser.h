#ifndef TPE_SO_INPUTPARSER_H
#define TPE_SO_INPUTPARSER_H

#include <stdint.h>
#include <stdio.h>

void findPipe(char * str, char ** part1, char ** part2);
int split(char * str, char argc, char ** args);

#endif // TPE_SO_INPUTPARSER_H
