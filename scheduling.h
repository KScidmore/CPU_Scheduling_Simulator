#ifndef SCHEDULING_H
#define SCHEDULING_H

#include "process.h"
#include "circular_queue.h"
#include "options.h"

void simulate_FCFS(Process processes[], int num_processes, Options *options);
void simulate_SJF(Process processes[], int num_processes, Options *options);
void simulate_priority(Process processes[], int num_processes, Options *options);
void simulate_SRTF(Process processes[], int num_processes);
void simulate_preemptive_priority(Process processes[], int num_processes);
void simulate_round_robin(Process processes[], int num_processes, int time_quantum);

void sort_queue(CircularQueue *queue, int (*compare)(const void *, const void *));
int compare_arrival(const void *a, const void *b);
int compare_burst(const void *a, const void *b);
int compare_completion(const void *a, const void *b);
int compare_priority(const void *a, const void *b);
int compare_predicted_burst(const void *a, const void *b);

void display_chart(Process processes[], int num_processes);
void display_chart_file(Process processes[], int num_processes);
void display_metrics(Process processes[], int num_processes, int idle_time, int current_time);
void display_preemptive_chart(Process processes[], int num_processes);





#endif