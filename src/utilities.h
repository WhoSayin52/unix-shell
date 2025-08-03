#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdbool.h>

typedef struct {
	char** array;
	int size;
	int capacity;
}ArrayString;

void print_err();

bool parse_command(char* input, ArrayString* args);

#endif //UTILITIES_H
