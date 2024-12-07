/*---------- ID HEADER -------------------------------------
/  Author(s):   Andrew Boisvert, Kyle Scidmore
/  Email(s):    abois526@mtroyal.ca, kscid125@mtroyal.ca 
/  File Name:   circular_queue.c
/
/  File Description:
/    This file implements a circular queue data structure and provides
/    functions to manipulate and display elements within it. The circular
/    queue supports operations such as enqueue, dequeue, display, and sort.
/    It uses a fixed-size array to store the elements and wraps the queue
/    around when the end of the array is reached.
/
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
/*---------- FUNCTION DOCUMENTATION BLOCK ------------------------------
/  Functions Overview:
/    These functions manage a circular queue which is used as the ready
/    queue for scheduling processes.
/
/  Functions and Their Purpose:
/    - init_queue: Initializes the circular queue by setting the front,
/      rear, and fill attributes to indicate an empty queue.
/    - isEmpty: Checks if the queue is empty.
/    - isFull: Checks if the queue is full.
/    - enqueue: Adds a process to the rear of the queue. If the queue
/      is full, it reports an error. If the queue is empty, it adjusts
/      the front pointer to the start.
/    - dequeue: Removes and returns the process at the front of the queue.
/
/  Input Parameters:
/    - All functions operate on a pointer to a CircularQueue structure and,
/      where applicable, take a Process structure as input.
/  
/  Output:
/    - The functions either return an integer value (for checking if the
/      queue is empty or full, or for removing and returning a process)
/      or modify the queue by enqueuing or dequeuing processes. 
/
/  Assumptions, Limitations, Known Bugs:
/    - No known bugs at this time.
/
/------------------------------------------------------------------------*/
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

/*---------- FUNCTION: display_queue ------------------------------------
/  Function Description:
/    Displays the elements of a circular queue. The function iterates through 
/    the queue and prints each process ID enclosed in square brackets, 
/    separated by commas.
/  
/  Caller Input:
/    - CircularQueue *queue: Pointer to the circular queue to be displayed.
/    - FILE *fp: File pointer where the queue elements should be written to.
/  
/  Caller Output:
/    N/A - No return value. The queue is displayed directly to the specified
/          file pointer.
/  
/  Assumptions, Limitations, Known Bugs:
/    - No known bugs at this time
/
------------------------------------------------------------------------*/
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
/          directly, modifying the original data in queue.
/  
/  Assumptions, Limitations, Known Bugs:
/    - No known bugs at this time
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