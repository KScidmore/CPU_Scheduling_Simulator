#ifndef SCHEDULING_H
#define SCHEDULING_H

#include "process.h"
#include "circular_queue.h"

void simulate_FCFS(Process processes[], int num_processes);
int compare_arrival(const void *a, const void *b);
void display_metrics(Process processes[], int num_processes, int idle_time, int current_time);
void simulate_SJF(Process processes[], int num_processes);
int compare_burst(const void *a, const void *b);
void sort_queue(CircularQueue *queue, int (*compare)(const void *, const void *));
int compare_completion(const void *a, const void *b);
void display_chart(Process processes[], int num_processes);

int compare_priority(const void *a, const void *b);
void simulate_priority(Process processes[], int num_processes);
#endif