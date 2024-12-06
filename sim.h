#ifndef SIM_H
#define SIM_H

#include "process.h"

int process_input(Process processes[], int choice);
int is_unique_id(Process processes[], int count, char *id);
#endif