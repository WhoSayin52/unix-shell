#ifndef COMMANDS_H
#define COMMANDS_H

#include "utilities.h"

#include <stdbool.h>

bool init_paths();

void destroy_paths();

bool execute_commnad(ArrayString* args);

#endif //COMMANDS_H
