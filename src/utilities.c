#include "utilities.h"

#include <string.h>
#include <unistd.h>
#include <stdlib.h>

void print_err() {
	char error_message[30] = "An error has occurred\n";
	write(STDERR_FILENO, error_message, strlen(error_message));
}

