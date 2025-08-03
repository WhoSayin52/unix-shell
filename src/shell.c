#define _POSIX_C_SOURCE 202409L
#define _DEFAULT_SOURCE

#include "shell.h"

#include "utilities.h"
#include "commands.h"

#include <unistd.h>
#
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define INPUT_BUFFER_SIZE 4096
#define MAX_ARGS 500

static char* input;
static ArrayString args;

void process_input(char* input);

void shell_init() {

	input = malloc(INPUT_BUFFER_SIZE + 1);
	if (input == NULL) {
		fprintf(stderr, "Not enough memory to initialize input buffer.\n");
		exit(1);
	}

	args.array = malloc(MAX_ARGS);
	if (args.array == NULL) {
		fprintf(stderr, "Not enough memory to initialize args.\n");
		free(input);
		exit(1);
	}
	args.capacity = MAX_ARGS;
	args.size = 1;
	args.array[0] = NULL;

	if (init_paths() == false) {
		fprintf(stderr, "Not enough memory to initialize paths array.\n");
		free(args.array);
		free(input);
		exit(1);
	}

	atexit(shell_destroy);
}

void shell_destroy() {
	destroy_paths();
	free(args.array);
	free(input);
}

void shell_interactive_mode() {
	bool is_running = true;

	while (is_running) {

		const char* prompt = "wish> ";
		size_t prompt_size = (size_t)strlen(prompt);

		ssize_t rc = write(STDOUT_FILENO, prompt, prompt_size);
		if (rc == -1) {
			print_err();
			continue;
		}

		fgets(input, INPUT_BUFFER_SIZE, stdin);

		process_input(input);
	}
}

void shell_batch_mode(char* file_path) {
	(void)file_path;
}

void process_input(char* input) {
	input[INPUT_BUFFER_SIZE] = '\0';

	char* sub_input;

	while ((sub_input = strsep(&input, "&"))[0] != '\0') {
		if (sub_input[0] == '\n') {
			return;
		}
		else {
			break;
		}
	}

	do {
		if (parse_command(sub_input, &args) == false) {
			print_err();
			continue;
		}

		if (execute_commnad(&args) == false) {
			print_err();
		}

	} while (((sub_input = strsep(&input, "&")) != NULL));
}

