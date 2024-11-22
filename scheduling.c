/*Using qsort: https://www.tutorialspoint.com/c_standard_library/c_function_qsort.htm*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"
#include "scheduling.h"
#include "globals.h"
#include "circular_queue.h"



void simulate_FCFS(Process processes[], int num_processes) {

    CircularQueue ready_queue;
    init_queue(&ready_queue);

    int current_time = 0;
    int idle_time = 0;

    qsort(processes, num_processes, sizeof(Process), compare_arrival);

    printf("\n\nTime\tEvent\t\t\tReady Queue\n");
    printf("---------------------------------------\n");

    int i = 0; 
    while (i < num_processes || !isEmpty(&ready_queue)) {
       
        /*Check if processes have arrived, stops when it finds a process that hasnt*/
        while (i < num_processes && processes[i].arrival_time <= current_time) {
            enqueue(&ready_queue, processes[i]);
            i++;
        }

        /*If queue is empty track idle state */
        if (isEmpty(&ready_queue)) {

            printf("%d\t\tIdle\t\t\t", current_time);
            display_queue(&ready_queue);
            printf("\n");
            current_time++;
            idle_time++;

        } else {
            Process current_process = dequeue(&ready_queue);

            current_process.start_time = current_time;
            printf("%d\t\tStarted P%s\t\t", current_time, current_process.id);
            display_queue(&ready_queue);
            printf("\n");

            /*Update current time and current processes metrics*/
            current_time += current_process.burst_time;

            for(int j = 0; j < num_processes; j++){

                if(strcmp(processes[j].id, current_process.id) == 0){
                    processes[j].completion_time = current_time;
                    processes[j].turnaround_time = current_time - processes[j].arrival_time;
                    processes[j].waiting_time = processes[j].turnaround_time - processes[j].burst_time;
                    processes[j].response_time = current_process.start_time - processes[j].arrival_time;
                    break;
                }
            }

            /*After process completes check if any processes arrived during execution or a time of completion*/
            while (i < num_processes && processes[i].arrival_time <= current_time) {
                enqueue(&ready_queue, processes[i]);
                i++;
            }

            printf("%d\t\tCompleted P%s\t", current_time, current_process.id);
            display_queue(&ready_queue);
            printf("\n");

        }
    }

    printf("---------------------------------------\n");
    printf("Simulation complete.\n\n");

    display_metrics(processes, num_processes, idle_time, current_time);

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