#include "utilities.h"

#include <string.h>
#include <unistd.h>
#include <stdlib.h>

void print_err() {
	char error_message[30] = "An error has occurred\n";
	write(STDERR_FILENO, error_message, strlen(error_message));
}

bool parse_command(char* input, ArrayString* args) {

	if (input[0] == '\0') {
		return false;
	}

	int size = 0;
	int capacity = args->capacity;
	char** arr = args->array;

	char* sub_input;
	while ((sub_input = strsep(&input, " \t\n")) != NULL) {

		if (size == capacity - 1) {
			return false;
		}

		if (sub_input[0] == '\0') {
			continue;
		}

		arr[size] = sub_input;
		++size;
	}

	arr[size] = NULL;
	++size;

	args->size = size;
	return true;
}
