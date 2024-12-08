/*---------- ID HEADER -------------------------------------
/  Author(s):   Andrew Boisvert, Kyle Scidmore
/  Email(s):    abois526@mtroyal.ca, kscid125@mtroyal.ca 
/  File Name:   compare.c
/
/  File Description:
/    This file contains functions used to compare processes based on different
/    scheduling criteria using the qsort function.
/---------------------------------------------------------*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "process.h"
#include "scheduling.h"
#include "globals.h"

/*---------- FUNCTION DOCUMENTATION BLOCK ------------------------------
/  Functions Overview:
/    Each function is designed to be used as a comparison function for `qsort`.
/  
/  Functions and Their Purpose:
/    - compare_arrival: Compares two processes based on their arrival time.
/    - compare_burst: Compares two processes based on their burst time. If 
/       burst times are equal, it compares their arrival times.
/    - compare_completion: Compares two processes based on their 
/       completion time.
/    - compare_priority: Compares two processes based on their priority. 
/       If priorities are equal, it compares their arrival times.
/    - compare_pid: Compares two processes based on the integer value 
/       of their pid string. 
/  
/  Input Parameters:
/    - All comparison functions take two void* pointers, a and b, which
/      are cast to Process* to perform the comparison.
/  
/  Output:
/    - Each comparison function returns an integer value:
/        * < 0: Indicates the first process is "less than" the second 
/        * 0: Indicates both processes are "equal" with respect to the 
/          comparison criteria.
/        * > 0: Indicates the first process is "greater than" the second.
/  
/  Assumptions, Limitations, Known Bugs:
/    - These functions are designed for use with qsort
/    - Assumes valid Process pointers are passed as arguments.
/    - No known bugs at this time.
/ 
/------------------------------------------------------------------------*/
int compare_arrival(const void *a, const void *b) {
    Process *process_a = (Process *)a;
    Process *process_b = (Process *)b;
    
    return process_a->arrival_time - process_b->arrival_time;
}

int compare_burst(const void *a, const void *b) {
    Process *process_a = (Process *)a;
    Process *process_b = (Process *)b;

    if (process_a->burst_time != process_b->burst_time) {
        return process_a->burst_time - process_b->burst_time;
    }

    
    return process_a->arrival_time - process_b->arrival_time;
}

int compare_completion(const void *a, const void *b) {
    Process *process_a = (Process *)a;
    Process *process_b = (Process *)b;
    return process_a->completion_time - process_b->completion_time;
}

int compare_priority(const void *a, const void *b) {
    Process *process_a = (Process *)a;
    Process *process_b = (Process *)b;

    if (process_a->priority != process_b->priority) {
        return process_a->priority - process_b->priority;
    }

    
    return process_a->arrival_time - process_b->arrival_time;
}

int compare_pid(const void *a, const void *b) {
    Process *process1 = (Process *)a;
    Process *process2 = (Process *)b;

    int pid1 = atoi(process1->id);
    int pid2 = atoi(process2->id);

    return pid1 - pid2;
}

int compare_remaining(const void *a, const void *b) {
    Process *process_a = (Process *)a;
    Process *process_b = (Process *)b;

    if (process_a->remaining_time != process_b->remaining_time) {
        return process_a->remaining_time - process_b->remaining_time;
    }

    
    return process_a->arrival_time - process_b->arrival_time;
}
