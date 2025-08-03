#ifndef SHELL_H
#define SHELL_H

void shell_init();

void shell_destroy();

void shell_interactive_mode();

void shell_batch_mode(char* file_path);

#endif //SHELL_H
