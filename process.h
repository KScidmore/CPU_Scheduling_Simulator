#ifndef PROCESS_H
#define PROCESS_H

typedef struct {
    char id[10];            // Process ID (User or File)
    int arrival_time;       // Time at which the process arrives (User or File)
    int burst_time;         // Total CPU burst time (User or File)
    int priority;           // Priority (lower value = higher priority) (User or File)
    int remaining_time;     // Remaining burst time (For RR and pre-emptive algos)
    int start_time;         // First time the process starts execution
    int completion_time;    // Time when the process finishes execution
    int turnaround_time;    // Turnaround time = completion_time - arrival_time
    int waiting_time;       // Waiting time = turnaround_time - burst_time
    int response_time;
} Process;


#endif