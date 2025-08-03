#define _POSIX_C_SOURCE 202409L 
#define _DEFAULT_SOURCE

#include "commands.h"

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define PATHS_SIZE 50
#define MAX_PATH_SIZE 256
#define MAX_FILE_NAME 256

typedef void (BuiltInFn)(char* argv[]);

struct built_in_fn_info {
	char* name;
	BuiltInFn* fn;
};

static ArrayString paths;

void path(char* argv[]);
void my_exit(char* argv[]);

int find_built_in_fn(char* command);

struct built_in_fn_info built_in_functions[] = {
	{ "exit", &my_exit },
	{ "path", &path }
};

bool init_paths() {
	paths.array = calloc(PATHS_SIZE, sizeof(char*));
	if (paths.array == NULL) {
		return false;
	}

	paths.capacity = PATHS_SIZE;
	paths.size = 1;
	paths.array[0] = strdup("/bin/");

	return true;
}

void destroy_paths() {
	for (int i = 0; i < paths.size; ++i) {
		free(paths.array[i]);
		paths.array[i] = NULL;
	}

	free(paths.array);
	paths.array = NULL;
	paths.size = 0;
	paths.capacity = 0;
}

bool execute_commnad(ArrayString* args) {

	char** argv = args->array;

	if (args->size == 1) {
		return false;
	}

	int built_in_index = find_built_in_fn(argv[0]);
	if (built_in_index >= 0) {
		built_in_functions[built_in_index].fn(&argv[1]);
		return true;
	}

	char full_path[MAX_PATH_SIZE + MAX_FILE_NAME + 1];
	full_path[MAX_PATH_SIZE + MAX_FILE_NAME] = '\0';

	for (int i = 0; i < paths.size; ++i) {
		strcpy(full_path, paths.array[i]);
		strcat(full_path, argv[0]);

		int rc = access(full_path, X_OK);

		if (rc == 0) {
			printf("WRITE EXE CODE\n");
			return true;
		}
	}

	return false;
}

int find_built_in_fn(char* command) {
	int built_in_fn_count = (int)sizeof(built_in_functions) / sizeof(built_in_functions[0]);

	for (int i = 0; i < built_in_fn_count; ++i) {
		int comp = strcmp(command, built_in_functions[i].name);
		if (comp == 0) {
			return i;
		}
	}
	return -1;
}

void path(char* argv[]) {

	/*
	for (int j = 0; j < paths.capacity; ++j) {
		if (paths.array[j] == NULL) {
			printf("%d- NULL\n", j);
		}
	}
	*/

	int i = 0;
	while (argv[i] != NULL && i < paths.capacity) {

		char* new_path = strdup(argv[i]);
		if (new_path == NULL) {
			fprintf(stderr, "Could not allocate memory to new path for paths.\n");
			if (i > paths.size) {
				paths.size = i;
			}
			exit(1);
		}
		free(paths.array[i]);
		paths.array[i] = new_path;
		++i;
	}

	paths.size = i;

	while (paths.array[i] != NULL && i < paths.capacity) {
		free(paths.array[i]);
		paths.array[i] = NULL;
		++i;
	}
}

void my_exit(char* argv[]) {
	(void)argv;
	exit(0);
}
