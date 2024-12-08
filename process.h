/*---------- ID HEADER -------------------------------------
/  Author(s):   Andrew Boisvert, Kyle Scidmore
/  Email(s):    abois526@mtroyal.ca, kscid125@mtroyal.ca 
/  File Name:   process.h
/
/  Program Purpose(s):
/    This header file describes the Process custom data type.
/---------------------------------------------------------*/

#ifndef PROCESS_H
#define PROCESS_H

/*---------- STRUCTURE: Process ----------------------------
/  Structure Description:
/    A custom data type representing a process. Contains attributes
/    and info on the current state to help the simulator track processes 
/    while they run using different scheduling algorithms and to calculate
/    performance metrics.
/    
/  Fields:
/    char id[10];            
/      Process ID 
/    int arrival_time;       
/      Time at which the process arrives 
/    int burst_time;         
/      Total CPU burst time 
/    int priority;           
/      Priority level (lower value = higher priority) 
/    int remaining_time;     
/      Remaining burst time (used for RR and pre-emptive algorithms)
/    int start_time;         
/      Time that the process starts execution
/    int completion_time;    
/      Time that the process finishes execution
/    int turnaround_time;    
/      Time to process to complete execution. 
/      Calculated as: turnaround_time = completion_time - arrival_time
/    int waiting_time;
/      Total time process waits in ready queue.       
/      Calculated as: waiting_time = turnaround_time - burst_time
/    int response_time;
/      Total time it takes a process to execute from its arrival. 
/      Calculated as: response_time = start_time - arrival_time 
/    int predicted_burst;
/      Predicted CPU burst time for preemptive algorithms.
/    int has_started;
/      Flag to indicate if a process has started executing (1 = true, 0 = false) 
/    int running_time[999];
/      Used to track the time slots when a process runs. 
/  Relationships:
/    - one of the most fundamental data structures of the program 
/    - CircularQueue maintains an array of this type to simulate the ready queue 
/      for the various scheduling algorithms
/---------------------------------------------------------*/
typedef struct {
    char id[10];            
    int arrival_time;       
    int burst_time;         
    int priority;
    int remaining_time;     
    int start_time;         
    int completion_time;    
    int turnaround_time;    
    int waiting_time;       
    int response_time;
    int predicted_burst;
    int has_started;
    int running_time[999];
} Process;


#endif