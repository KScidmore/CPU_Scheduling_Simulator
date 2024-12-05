/*Using qsort: https://www.tutorialspoint.com/c_standard_library/c_function_qsort.htm*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"
#include "scheduling.h"
#include "globals.h"
#include "circular_queue.h"
#include "ansi_esc_seqs.h"
#include "options.h"



void simulate_FCFS(Process processes[], int num_processes, Options *options, FILE *fp) {

    CircularQueue ready_queue;
    init_queue(&ready_queue);

    int current_time = 0;
    int idle_time = 0;
    int start_time;

    qsort(processes, num_processes, sizeof(Process), compare_arrival);

    fprintf(fp, "\n\nTime\tEvent\t\tReady Queue\n");
    fprintf(fp, "---------------------------------------\n");

    int i = 0; 
    while (i < num_processes || !isEmpty(&ready_queue)) {
       
        /*Check if processes have arrived, stops when it finds a process that hasnt*/
        while (i < num_processes && processes[i].arrival_time <= current_time) {
            enqueue(&ready_queue, processes[i]);
            i++;
        }

        /*If queue is empty track idle state */
        if (isEmpty(&ready_queue)) {

            fprintf(fp, "%d\tIdle\t\t", current_time);
            display_queue(&ready_queue, fp);
            fprintf(fp, "\n");
            current_time++;
            idle_time++;

        } else {
            Process current_process = ready_queue.data[ready_queue.front];

            start_time = current_time;
            fprintf(fp, "%d\tStarted P%s\t", current_time, current_process.id);
            display_queue(&ready_queue, fp);
            fprintf(fp, "\n");

            dequeue(&ready_queue);
            /*Update current time and current processes metrics*/
            current_time += current_process.burst_time;

            for(int j = 0; j < num_processes; j++){

                if(strcmp(processes[j].id, current_process.id) == 0){
                    processes[j].completion_time = current_time;
                    processes[j].turnaround_time = current_time - processes[j].arrival_time;
                    processes[j].waiting_time = processes[j].turnaround_time - processes[j].burst_time;
                    processes[j].response_time = start_time - processes[j].arrival_time;
                    processes[j].start_time = start_time;
                    break;
                }
            }

            /*After process completes check if any processes arrived during execution or a time of completion*/
            while (i < num_processes && processes[i].arrival_time <= current_time) {
                enqueue(&ready_queue, processes[i]);
                i++;
            }

            fprintf(fp, "%d\tCompleted P%s\t", current_time, current_process.id);
            display_queue(&ready_queue, fp);
            fprintf(fp, "\n");

        }
    }

    printf("---------------------------------------\n");
    printf("Simulation complete.\n\n");

    qsort(processes, num_processes, sizeof(Process), compare_completion);

    display_metrics(processes, num_processes, idle_time, current_time, fp);

    display_chart(processes, num_processes, fp);

}


void simulate_SJF(Process processes[], int num_processes, Options *options, FILE *fp) {


    CircularQueue ready_queue;
    init_queue(&ready_queue);

    int current_time = 0;
    int idle_time = 0;
    int start_time;

    printf("\nRunning Simulation for SJF (Shortest Job First)\n\n");
    qsort(processes, num_processes, sizeof(Process), compare_arrival);

    printf("\n\nTime\tEvent\t\tReady Queue\n");
    printf("---------------------------------------\n");

    int i = 0;
    while (i < num_processes || !isEmpty(&ready_queue)) {

        while (i < num_processes && processes[i].arrival_time <= current_time) {
            enqueue(&ready_queue, processes[i]);
            i++;
        }

        if (isEmpty(&ready_queue)) {

            printf("%d\tIdle\t\t", current_time);
            display_queue(&ready_queue, fp);
            printf("\n");
            current_time++;
            idle_time++;
        } else {

            sort_queue(&ready_queue, compare_burst);

            Process current_process = ready_queue.data[ready_queue.front];

            start_time = current_time;
            printf("%d\tStarted P%s\t", current_time, current_process.id);
            display_queue(&ready_queue, fp);
            printf("\n");
            dequeue(&ready_queue);
            current_time += current_process.burst_time;

            for (int j = 0; j < num_processes; j++) {
                if (strcmp(processes[j].id, current_process.id) == 0) {
                    processes[j].completion_time = current_time;
                    processes[j].turnaround_time = current_time - processes[j].arrival_time;
                    processes[j].waiting_time = processes[j].turnaround_time - processes[j].burst_time;
                    processes[j].response_time = start_time - processes[j].arrival_time;
                    processes[j].start_time = start_time;
                    break;
                }
            }

            while (i < num_processes && processes[i].arrival_time <= current_time) {
                enqueue(&ready_queue, processes[i]);
                i++;
            }

            printf("%d\tCompleted P%s\t", current_time, current_process.id);
            display_queue(&ready_queue, fp);
            printf("\n");
        }
    }

    printf("---------------------------------------\n");
    printf("Simulation complete.\n\n");

    qsort(processes, num_processes, sizeof(Process), compare_completion);

    display_metrics(processes, num_processes, idle_time, current_time, fp);

    display_chart(processes, num_processes, fp);

}


void simulate_priority(Process processes[], int num_processes, Options *options, FILE *fp) {
    
    FILE *output = stdout;
    if (options->output_file[0] != '\0') {
        output = fopen(options->output_file, "w");
    }

    CircularQueue ready_queue;
    init_queue(&ready_queue);

    int current_time = 0;
    int idle_time = 0;
    int start_time;

    printf("\nRunning Simulation for Priority Scheduling\n\n");
    qsort(processes, num_processes, sizeof(Process), compare_arrival);

    printf("\n\nTime\tEvent\t\tReady Queue\n");
    printf("---------------------------------------\n");

    int i = 0;
    while (i < num_processes || !isEmpty(&ready_queue)) {

        while (i < num_processes && processes[i].arrival_time <= current_time) {
            enqueue(&ready_queue, processes[i]);
            i++;
        }

        if (isEmpty(&ready_queue)) {

            printf("%d\tIdle\t\t", current_time);
            display_queue(&ready_queue, fp);
            printf("\n");
            current_time++;
            idle_time++;
        } else {

            sort_queue(&ready_queue, compare_priority);

            Process current_process = ready_queue.data[ready_queue.front];

            start_time = current_time;
            printf("%d\tStarted P%s\t", current_time, current_process.id);
            display_queue(&ready_queue, fp);
            printf("\n");
            dequeue(&ready_queue);
            current_time += current_process.burst_time;

            for (int j = 0; j < num_processes; j++) {
                if (strcmp(processes[j].id, current_process.id) == 0) {
                    processes[j].completion_time = current_time;
                    processes[j].turnaround_time = current_time - processes[j].arrival_time;
                    processes[j].waiting_time = processes[j].turnaround_time - processes[j].burst_time;
                    processes[j].response_time = start_time - processes[j].arrival_time;
                    processes[j].start_time = start_time;
                    break;
                }
            }

            while (i < num_processes && processes[i].arrival_time <= current_time) {
                enqueue(&ready_queue, processes[i]);
                i++;
            }

            printf("%d\tCompleted P%s\t", current_time, current_process.id);
            display_queue(&ready_queue, fp);
            printf("\n");
        }
    }

    printf("---------------------------------------\n");
    printf("Simulation complete.\n\n");

    qsort(processes, num_processes, sizeof(Process), compare_completion);

    display_metrics(processes, num_processes, idle_time, current_time, fp);

    display_chart(processes, num_processes, fp);

}

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

void display_metrics(Process processes[], int num_processes, int idle_time, int current_time, FILE *fp) {
    int total_waiting = 0;
    int total_turnaround = 0;
    int total_response = 0; 
    int total_completion = current_time;

    for (int i = 0; i < num_processes; i++) {
        fprintf(fp, "Metrics for P%s: Turnaround= %d, Waiting= %d, Response= %d\n",
                processes[i].id,
                processes[i].turnaround_time,
                processes[i].waiting_time,
                processes[i].response_time);
    }

    for (int i = 0; i < num_processes; i++) {
        total_waiting += processes[i].waiting_time;
        total_turnaround += processes[i].turnaround_time;
        total_response += processes[i].response_time;
    }

    float avg_waiting = (float)total_waiting / num_processes;
    float avg_turnaround = (float)total_turnaround / num_processes;
    float avg_response = (float)total_response / num_processes; 
    float throughput = (float)num_processes / total_completion;
    float cpu_utilization = (total_completion - idle_time) / (float)total_completion * 100;

    fprintf(fp, "\nMetrics Summary:\n");
    fprintf(fp, "Average Waiting Time: %.2f\n", avg_waiting);
    fprintf(fp, "Average Turnaround Time: %.2f\n", avg_turnaround);
    fprintf(fp, "Average Response Time: %.2f\n", avg_response);
    fprintf(fp, "Throughput: %.2f processes per time unit\n", throughput);
    fprintf(fp, "CPU Utilization: %.2f%%\n", cpu_utilization);
}


void display_chart(Process processes[], int num_processes, FILE *fp) {

    int max_time = 0;
    int current_time = 0;

    for (int i = 0; i < num_processes; i++) {
        if (processes[i].completion_time > max_time) {
            max_time = processes[i].completion_time;
        }
    }

    fprintf(fp, "\nGantt Chart:\n");
    fprintf(fp, ANSI_BOLD ANSI_BLUE "--------------------------------------------" ANSI_RESET "\n");

    for (int start = 0; start <= max_time; start += MAX_WIDTH) {

        int end = (start + MAX_WIDTH - 1 < max_time) ? start + MAX_WIDTH - 1 : max_time;

        
        fprintf(fp, ANSI_BOLD ANSI_BLUE "Time: " ANSI_RESET);
        for (int t = start; t <= end; t++) {
            fprintf(fp, ANSI_BOLD ANSI_BLUE "%-4d" ANSI_RESET, t);  
        }
        fprintf(fp, "\n");

        
        for (int i = 0; i < num_processes; i++) {
            const char* color;
            switch (i % 5) {
                case 0: color = ANSI_RED; break;
                case 1: color = ANSI_GREEN; break;
                case 2: color = ANSI_YELLOW; break;
                case 3: color = ANSI_BLUE; break;
                case 4: color = ANSI_MAGENTA; break;
            }
            fprintf(fp, "%sP%-2s| " ANSI_RESET, color, processes[i].id);  

            for (int j = start; j <= end; j++) {
                if (j >= processes[i].start_time && j <= processes[i].completion_time) {
                    
                    fprintf(fp, "%s### " ANSI_RESET, color); 
                    
                } else {
                    fprintf(fp, "    "); 
                }
                current_time++;
            }
            fprintf(fp, "\n");
        }

        fprintf(fp, ANSI_BOLD ANSI_BLUE "--------------------------------------------" ANSI_RESET "\n");
    }
}

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

    /*If Burst times equal compare by arrival*/
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

    /*If priority equal compare by arrival*/
    return process_a->arrival_time - process_b->arrival_time;
}