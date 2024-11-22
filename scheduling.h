#ifndef SCHEDULING_H
#define SCHEDULING_H

#include "process.h"

void simulate_FCFS(Process processes[], int num_processes);
int compare_arrival(const void *a, const void *b);
void display_metrics(Process processes[], int num_processes, int idle_time, int current_time);

#endif