#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "process.h"
#include "scheduling.h"
#include "globals.h"

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