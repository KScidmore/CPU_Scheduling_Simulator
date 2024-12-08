/*---------- ID HEADER -------------------------------------
/  Author(s):   Andrew Boisvert, Kyle Scidmore
/  Email(s):    abois526@mtroyal.ca, kscid125@mtroyal.ca 
/  File Name:   circular_queue.h
/
/  Program Purpose(s):
/    This header file describes the public interface of the accompanying
/    ".c" file, describing how this module may be used by others. 
/---------------------------------------------------------*/

#ifndef CIRCULAR_QUEUE_H
#define CIRCULAR_QUEUE_H

#include "globals.h"
#include "process.h"

/*---------- STRUCTURE: CircularQueue ----------------------
/  Structure Description:
/    A custom data type representing a circular-wrapping queue
/    of Processes. Useful for simulating the ready-queue of 
/    the process scheduling algorithms. 
/  
/  Members:
/    Process data[MAX_PROCESSES]
/      The contents of the queue: an array of Process structs
/    int front
/      represents the index of the front end of the queue 
/    int rear
/      represents the index of the rear end of the queue 
/    int fill
/      the number of elements currently in the queue  
/  
/  Relationships:
/    - contains an array of Process structs 
/---------------------------------------------------------*/
typedef struct {
    Process data[MAX_PROCESSES];
    int front;
    int rear;
    int fill;
} CircularQueue;

void init_queue(CircularQueue *queue);
int isEmpty(CircularQueue *queue);
int isFull(CircularQueue *queue);
void enqueue(CircularQueue *queue, Process process);
Process dequeue(CircularQueue *queue);
void display_queue(CircularQueue *queue);
void sort_queue(CircularQueue *queue, int (*compare)(const void *, const void *));

#endif