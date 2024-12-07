/*---------- ID HEADER -------------------------------------
/  Author(s):   Andrew Boisvert, Kyle Scidmore
/  Email(s):    abois526@mtroyal.ca, kscid125@mtroyal.ca 
/  File Name:   circular_queue.c
/
/  Program Description:
/    TODO - a short, high-level overview of the program's purpose
/    from the perspective of the user (what it does, not how it does it)
/---------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "circular_queue.h"
#include "globals.h"
#include "process.h"
#include "compare.h"

void circ_queue_test_1();
void init_process(Process *process, int proc_id);
void peek_front(CircularQueue *queue);
void peek_rear(CircularQueue *queue);


/*---------- Public Functions Used Outside Module ----------------------------*/

void init_queue(CircularQueue *queue) {
    queue->front = -1;
    queue->rear = -1;
    queue->fill = 0;
}

int isEmpty(CircularQueue *queue) {
    return queue->fill == 0;
}


int isFull(CircularQueue *queue) {
    return queue->fill == MAX_PROCESSES;
}


void enqueue(CircularQueue *queue, Process process) {
    if (isFull(queue)) {
        fprintf(stderr, "Error: queue is full, cannot enqueue.\n");
        return;
    } else if (isEmpty(queue)) {
        queue->front++;
    }
    queue->rear = (queue->rear + 1) % MAX_PROCESSES;
    queue->data[queue->rear] = process;
    queue->fill++;
}

Process dequeue(CircularQueue *queue) {
    if (isEmpty(queue)) {
        fprintf(stderr, "Error: queue is empty, cannot dequeue.\n");
    } else {
        Process process = queue->data[queue->front];
        queue->front = (queue->front + 1) % MAX_PROCESSES;
        queue->fill--;

        if (queue->fill == 0) {
            queue->front = -1;
            queue->rear = -1;
        }

        return process;
    }
}

void display_queue(CircularQueue *queue, FILE *fp) {

    fprintf(fp, "[");
    if (!isEmpty(queue)) {
        for (int i = 0; i < queue->fill; i++) {
            int index = (queue->front + i) % MAX_PROCESSES;
            fprintf(fp, "P%s", queue->data[index].id); // Add "P" before the process ID
            if (i < queue->fill - 1) fprintf(fp, ", ");
        }
    }
    fprintf(fp, "]");
}

/*---------- FUNCTION: sort_queue -------------------------------------
/  Function Description:
/    Sorts the elements in a circular queue using a comparison function. 
/    The function extracts all elements from the circular queue, sorts 
/    them in a temporary array, and then reinserts them back into the 
/    queue in sorted order.
/  
/  Caller Input:
/    - CircularQueue *queue: Pointer to the circular queue to be sorted.
/    - int (*compare)(const void *, const void *): Pointer to a comparison 
/      function compatible with `qsort`, used to define the sorting order.
/  
/  Caller Output:
/    N/A - No return value or output parameters. The queue is sorted
/          directly, modifying the original data in `queue`.
/  
/  Assumptions, Limitations, Known Bugs:
/    - Assumes `queue` is a valid, properly initialized circular queue.
/    - Assumes `queue->fill` accurately reflects the number of elements
/      in the queue, and `queue->front` points to the first element.
/    - Assumes the provided `compare` function adheres to `qsort` 
/      requirements (returning negative, zero, or positive values).
/    - If `queue->fill <= 1`, the function returns immediately as sorting 
/      is unnecessary.
/    - Assumes the size of the queue does not exceed `MAX_PROCESSES`.
/    - Known limitation: Temporarily uses extra memory equal to the size 
/      of the queue (proportional to `MAX_PROCESSES`).
/  
/  Notes:
/    - This function is generic and can be used to sort a circular queue
/      based on various criteria by providing a different comparison function.
/    - Reinserts elements in the queue's logical order, preserving
/      its circular nature.
/    - Example usage:
/          sort_queue(&queue, compare_arrival);
/      where `compare_arrival` is a function to sort by arrival time.
/    - Time complexity: O(n log n) due to `qsort`.
/
------------------------------------------------------------------------*/

void sort_queue(CircularQueue *queue, int (*compare)(const void *, const void *)) {
    if (queue->fill <= 1) return; 

    Process temp_array[MAX_PROCESSES];
    int index = queue->front;
    for (int i = 0; i < queue->fill; i++) {
        temp_array[i] = queue->data[index];
        index = (index + 1) % MAX_PROCESSES; 
    }

    
    qsort(temp_array, queue->fill, sizeof(Process), compare);


    for (int i = 0; i < queue->fill; i++) {
        queue->data[(queue->front + i) % MAX_PROCESSES] = temp_array[i];
    }
}

/*---------- Helper Functions Not Used Outside Module ------------------------*/

void circ_queue_test_1() {

    FILE *fp = stdout;

    int i;

    Process processes[MAX_PROCESSES];
    for (i = 0; i < MAX_PROCESSES; i++) {
        init_process(&processes[i], i + 1);
    }

    CircularQueue ready_queue;
    init_queue(&ready_queue);


    /* check if empty and/or full */
    printf("Checking if queue is empty: ");
    isEmpty(&ready_queue) ? printf("true\n") : printf("false\n");
    printf("Checking if queue is full: ");
    isFull(&ready_queue) ? printf("true\n") : printf("false\n");
    printf("\n");
    
    /* enqueue first process */
    printf("Enqueuing an item.\n");
    enqueue(&ready_queue, processes[0]);
    
    /* check if empty and/or full */
    printf("Checking if queue is empty: ");
    isEmpty(&ready_queue) ? printf("true\n") : printf("false\n");
    printf("Checking if queue is full: ");
    isFull(&ready_queue) ? printf("true\n") : printf("false\n");
    printf("\n");
    
    /* enqueue more */
    for (i = 1; i < MAX_PROCESSES - 1; i++) {
        printf("Enqueuing an item.\n");
        enqueue(&ready_queue, processes[i]);
    }
    
    /* check if empty and/or full */
    printf("Checking if queue is empty: ");
    isEmpty(&ready_queue) ? printf("true\n") : printf("false\n");
    printf("Checking if queue is full: ");
    isFull(&ready_queue) ? printf("true\n") : printf("false\n");
    printf("\n");
    
    /* enqueue last process */
    printf("Enqueuing an item.\n");
    enqueue(&ready_queue, processes[MAX_PROCESSES - 1]);
    printf("Displaying current queue: ");
    display_queue(&ready_queue, fp);
    printf("\n\n");
    
    /* check if empty and/or full */
    printf("Checking if queue is empty: ");
    isEmpty(&ready_queue) ? printf("true\n") : printf("false\n");
    printf("Checking if queue is full: ");
    isFull(&ready_queue) ? printf("true\n") : printf("false\n");
    printf("\n");
    
    /* enqueue out of bounds process */
    printf("Enqueuing out of top bound.\n");
    enqueue(&ready_queue, processes[MAX_PROCESSES]);
    printf("Displaying current queue: ");
    display_queue(&ready_queue, fp);
    printf("\n\n");

    /* peek at front and rear */
    peek_front(&ready_queue);
    peek_rear(&ready_queue);
    
    /* dequeue processes */
    for (i = 0; i < MAX_PROCESSES; i++) {
        printf("Dequeuing an item.\n");
        dequeue(&ready_queue);
        printf("Displaying current queue: ");
        display_queue(&ready_queue, fp);
        printf("\n\n");
        /* peek at front and rear */
        peek_front(&ready_queue);
        peek_rear(&ready_queue);
    }

    /* dequeue out of bounds */
    printf("Dequeuing an item.\n");
    dequeue(&ready_queue);
    
}

void peek_front(CircularQueue *queue) {
    if (isEmpty(queue)) {
        fprintf(stderr, "Error: queue is empty, cannot peek at front item.\n");
    } else {
        printf("Peeking at front: %s\n\n", queue->data[queue->front].id);
    }
}

void peek_rear(CircularQueue *queue) {
    if (isEmpty(queue)) {
        fprintf(stderr, "Error: queue is empty, cannot peek at rear item.\n");
    } else {
        printf("Peeking at rear: %s\n\n", queue->data[queue->rear].id);
    }
}

void init_process(Process *process, int proc_id) {
    snprintf(process->id, sizeof(process->id), "P%d", proc_id);
    process->arrival_time = 0;     
    process->burst_time = 0;     
    process->priority = -1; 
    process->remaining_time = 0;
    process->start_time = -1;
    process->completion_time = 0;   
    process->turnaround_time = 0;   
    process->waiting_time = 0;
    process->response_time = -1;
}