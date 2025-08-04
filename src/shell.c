#define _POSIX_C_SOURCE 202409L
#define _DEFAULT_SOURCE

//#define NDEBUG

#include "shell.h"

#include "utilities.h"
#include "commands.h"

#include <stdlib.h>
#include <stdbool.h>	
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>

#define INPUT_BUFFER 4096
#define MAX_ARGS 50

static char* input = NULL;
static ArrayString args;

static void init();
static void destroy();
static void interactive_mode();
static void batch_mode(char* file_path);

static void process_input(char* input);
static bool parse_sub_input();

void shell_run(int argc, char* argv[]) {

	if (argc > 2) {
		print_err();
		exit(1);
	}

	init();

	if (argc == 1) {
#ifndef NDEBUG
		printf("interactive_mode()\n");
#endif //NDEBUG
		interactive_mode();
	}
	else {
#ifndef NDEBUG
		printf("batch_mode(%s)\n", argv[1]);
#endif //NDEBUG
		batch_mode(argv[1]);
	}
}

static void init() {
	input = malloc(INPUT_BUFFER);
	if (input == NULL) {
		fprintf(stderr, "Could not allocate enough memeory for input buffer.\n");
		exit(1);
	}

	args.array = malloc((MAX_ARGS + 1) * sizeof(char*));
	if (args.array == NULL) {
		fprintf(stderr, "Could not allocate enough memeory for args.\n");
		free(input);
		exit(1);
	}
	args.array[0] = NULL;
	args.size = 0;
	args.capacity = MAX_ARGS + 1;

	if (init_paths() == false) {
		fprintf(stderr, "Could not allocate enough memeory for paths.\n");
		free(args.array);
		free(input);
		exit(1);
	}

	atexit(destroy);
}

static void destroy() {
	destroy_paths();
	free(args.array);
	free(input);
}

static void interactive_mode() {
	char* prompt = "wish> ";

	bool is_running = true;
	while (is_running) {
		printf("%s", prompt);

		char* read = fgets(input, INPUT_BUFFER, stdin);

		if (read == NULL) {
			print_err();
			continue;
		}

		process_input(input);
	}
}

static void batch_mode(char* file_path) {
	printf("%s\n", file_path);
}

static void process_input(char* input) {

#ifndef NDEBUG
	printf("input: %s\n", input);
#endif //NDEBUG

	while (input[0] != '\0') {
		if (input[0] != ' ' && input[0] != '\t') {
			break;
		}
		++input;
	}

	if (input[0] == '\n') {
		return;
	}

	char* sub_input = NULL;

	while ((sub_input = strsep(&input, "&")) != NULL) {
#ifndef NDEBUG
		printf("sub-input: %s\n", sub_input);
#endif //NDEBUG

		if (sub_input[0] == '\0') {
			print_err();
			continue;
		}

		if (parse_sub_input(&sub_input) == false) {
			print_err();
			continue;
		}

		if (execute_commnad(&args) == false) {
			print_err();
			continue;
		}
	}

	errno = 0;
	while (true) {
		if (wait(NULL) == -1) {
			if (errno == ECHILD) {
				break;
			}
		}
	}
}

static bool parse_sub_input(char** sub_input) {

	args.size = 0;

	char* arg = NULL;
	while ((arg = strsep(sub_input, " \t\n")) != NULL) {

#ifndef NDEBUG
		printf("arg: %s\n", arg);
#endif //NDEBUG

		if (arg[0] == '\0') {
			continue;
		}

		if (args.size == MAX_ARGS) {
			return false;
		}

		args.array[args.size] = arg;
		++args.size;

#ifndef NDEBUG
		printf("args[%d]: %s\n", args.size - 1, arg);
#endif //NDEBUG
	}

	args.array[args.size] = NULL;

	return true;
}
