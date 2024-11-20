/*Using qsort: https://www.tutorialspoint.com/c_standard_library/c_function_qsort.htm*/

#include <stdio.h>
#include <stdlib.h>
#include "process.h"
#include "scheduling.h"
#include "globals.h"



void simulate_FCFS(Process processes[], int num_processes) {

    Process ready_queue[MAX_PROCESSES];
    Process completed_processes[MAX_PROCESSES];
    int completed_index = 0;
    int current_time = 0;
    int idle_time = 0;
    int queue_size = 0;

    qsort(processes, num_processes, sizeof(Process), compare_arrival);

    printf("\nTime\tEvent\t\tReady Queue\n");
    printf("---------------------------------------\n");

    int i = 0; 
    while (i < num_processes || queue_size > 0) {
       
        /*Check if processes have arrived, stops when it finds a process that hasnt*/
        while (i < num_processes && processes[i].arrival_time <= current_time) {
            ready_queue[queue_size++] = processes[i];
            i++;
        }

        /*If queue is empty track idle state */
        if (queue_size == 0) {

            printf("%d\tIdle\t\t", current_time);
            display_queue(ready_queue, queue_size);
            printf("\n");
            current_time++;
            idle_time++;

        } else {

            Process current_process = ready_queue[0];

            current_process.start_time = current_time;
            printf("%d\tStarted P%s\t", current_time, current_process.id);
            display_queue(ready_queue, queue_size);
            printf("\n");

            /*Update current time and current processes metrics*/
            current_time += current_process.burst_time;
            current_process.completion_time = current_time;
            current_process.turnaround_time = current_process.completion_time - current_process.arrival_time;
            current_process.waiting_time = current_process.turnaround_time - current_process.burst_time;
            current_process.response_time = current_process.start_time - current_process.arrival_time;

            /*Shift processes in ready queue*/
            for (int j = 1; j < queue_size; j++) {
                ready_queue[j - 1] = ready_queue[j];
            }
            queue_size--;

            /*After process completes check if any processes arrived during execution or a time of completion*/
            while (i < num_processes && processes[i].arrival_time <= current_time) {
                ready_queue[queue_size++] = processes[i];
                i++;
            }

            printf("%d\tCompleted P%s\t", current_time, current_process.id);
            display_queue(ready_queue, queue_size);
            printf("\n");

            /*Add current process to final array of completed processes*/
            completed_processes[completed_index++] = current_process;

        }
    }

    printf("---------------------------------------\n");
    printf("Simulation complete.\n\n");

    display_metrics(completed_processes, num_processes, idle_time, current_time);

}



void display_metrics(Process processes[], int num_processes, int idle_time, int current_time){

    int total_waiting = 0;
    int total_turnaround = 0;
    int total_completion = current_time;

    for(int i = 0; i < num_processes; i++){
        printf("Metrics for P%s: Turnaround= %d, Waiting= %d, Response= %d\n",
                processes[i].id,
                processes[i].turnaround_time,
                processes[i].waiting_time,
                processes[i].response_time);
    }

    for (int i = 0; i < num_processes; i++) {
        total_waiting += processes[i].waiting_time;
        total_turnaround += processes[i].turnaround_time;
    }

    float avg_waiting = (float) total_waiting / num_processes;
    float avg_turnaround = (float) total_turnaround / num_processes;
    float throughput = (float) num_processes / total_completion;
    float cpu_utilization = (total_completion - idle_time) / (float) total_completion * 100;

    printf("\nMetrics Summary:\n");
    printf("Average Waiting Time: %.2f\n", avg_waiting);
    printf("Average Turnaround Time: %.2f\n", avg_turnaround);
    printf("Throughput: %.2f processes per time unit\n", throughput);
    printf("CPU Utilization: %.2f%%\n", cpu_utilization);
}

int compare_arrival(const void *a, const void *b) {
    Process *process_a = (Process *)a;
    Process *process_b = (Process *)b;
    
    return process_a->arrival_time - process_b->arrival_time;
}

void display_queue(Process ready_queue[], int queue_size) {
    printf("[");
    for (int i = 0; i < queue_size; i++) {
        printf("P%s", ready_queue[i].id);
        if (i < queue_size - 1) printf(", ");
    }
    printf("]");
}