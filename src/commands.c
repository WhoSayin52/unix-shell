#define _POSIX_C_SOURCE 202409L 
#define _DEFAULT_SOURCE

#include "commands.h"

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>

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
void cd(char* argv[]);
void my_exit(char* argv[]);

int find_built_in_fn(char* command);

struct built_in_fn_info built_in_functions[] = {
	{ "path", &path },
	{ "cd", &cd },
	{ "exit", &my_exit }
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

	if (args->size == 0) {
		return false;
	}

	char** argv = args->array;

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

		int rc_access = access(full_path, X_OK);

		if (rc_access == 0) {
#ifndef NDEBUG

			printf("External program to run: %s\n", full_path);

#endif //NDEBUG

			int rc_fork = fork();
			if (rc_fork == -1) {
				return false;
			}

			if (rc_fork == 0) {
				execv(full_path, argv);
				print_err();
				exit(1);
			}

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

#ifndef NDEBUG

	int index = 0;
	char* word = argv[index];
	printf("NEW PATHS ARGV: ");
	while (word != NULL) {
		printf("%s : ", word);
		word = argv[++index];
	}
	printf("\n");

#endif //NDEBUG

	int i = 0;
	while (i < paths.capacity) {
		char* path = *argv;

		if (path == NULL) {
			break;
		}

		++argv;

		int path_len = (int)strlen(path);
		if (path_len + 1 > MAX_PATH_SIZE) {
			print_err();
			continue;
		}

		path = strdup(path);

		if (path == NULL) {
			fprintf(stderr, "Could not allocate enought memory for a new path");
			exit(1);
		}

		int rc = access(path, R_OK);
		if (rc != 0) {
			free(path);
			print_err();
			continue;
		}

#ifndef NDEBUG

		printf("PATH: %s\n", path);

#endif //NDEBUG

		char* old_path = paths.array[i];
		if (old_path != NULL) {
			free(old_path);
		}

		paths.array[i] = path;
		++i;
	}

	int new_size = i;

	while (i < paths.size) {
		free(paths.array[i]);
		++i;
	}

	paths.size = new_size;

#ifndef NDEBUG

	printf("UPDATED PATHS: ");
	for (int j = 0; j < paths.size; ++j) {
		printf("%s: ", paths.array[j]);
	}
	printf("\n");

#endif //NDEBUG
}

void cd(char* argv[]) {

	char* path = argv[0];
	if (path == NULL) {
		print_err();
		return;
	}

	if (argv[1] != NULL) {
		print_err();
		return;
	}

#ifndef NDEBUG

	printf("New dir path: %s\n", path);

#endif //NDEBUG

	int rc = chdir(path);

	if (rc != 0) {
		print_err();
		return;
	}

#ifndef NDEBUG

	char cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)) != NULL) {
		printf("Current working directory: %s\n", cwd);
	}

#endif //NDEBUG
}

void my_exit(char* argv[]) {
	(void)argv;
	exit(0);
}
