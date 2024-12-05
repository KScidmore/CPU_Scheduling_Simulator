/*---------- ID HEADER -------------------------------------
/  Author(s):   Andrew Boisvert, Kyle Scidmore
/  Email(s):    abois526@mtroyal.ca, kscid125@mtroyal.ca 
/  File Name:   circular_queue.h
/
/  Program Purpose(s):
/    User-defined header file for circular_queue.c. 
/---------------------------------------------------------*/

#ifndef CIRCULAR_QUEUE_H
#define CIRCULAR_QUEUE_H

#include "globals.h"
#include "process.h"

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

#endif