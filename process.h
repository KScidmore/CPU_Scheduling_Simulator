#ifndef PROCESS_H
#define PROCESS_H

typedef struct {
    char id[10];
    int arrival_time;
    int burst_time;
    int priority;
    int waiting_time;
    int turnaround_time;
    int response_time;
    int remaining_time;
} Process;

#endif