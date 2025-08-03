#include "shell.h"
#include "utilities.h"

#include <stdlib.h>

int main(int argc, char* argv[]) {

	if (argc > 2) {
		print_err();
		exit(1);
	}

	shell_init();

	if (argc == 1) {
		shell_interactive_mode();
	}
	else {
		shell_batch_mode(argv[1]);
	}

	shell_destroy();

	return 0;
}
