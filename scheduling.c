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


void simulate_FCFS_to_stdout(Process processes[], int num_processes) {

    CircularQueue ready_queue;
    init_queue(&ready_queue);

    int current_time = 0;
    int idle_time = 0;
    int start_time;

    qsort(processes, num_processes, sizeof(Process), compare_arrival);

    printf("\n\nTime\tEvent\t\tReady Queue\n");
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

            printf("%d\tIdle\t\t", current_time);
            display_queue(&ready_queue);
            printf("\n");
            current_time++;
            idle_time++;

        } else {
            Process current_process = ready_queue.data[ready_queue.front];

            start_time = current_time;
            printf("%d\tStarted P%s\t", current_time, current_process.id);
            display_queue(&ready_queue);
            printf("\n");

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

            printf("%d\tCompleted P%s\t", current_time, current_process.id);
            display_queue(&ready_queue);
            printf("\n");

        }
    }

    printf("---------------------------------------\n");
    printf("Simulation complete.\n\n");

    qsort(processes, num_processes, sizeof(Process), compare_completion);

    display_metrics(processes, num_processes, idle_time, current_time);

    display_chart(processes, num_processes);

}

/* TODO */
void simulate_FCFS_to_file(Process processes[], int num_processes, Options *options) {

    CircularQueue ready_queue;
    init_queue(&ready_queue);

    int current_time = 0;
    int idle_time = 0;
    int start_time;

    qsort(processes, num_processes, sizeof(Process), compare_arrival);

    printf("\n\nTime\tEvent\t\tReady Queue\n");
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

            printf("%d\tIdle\t\t", current_time);
            display_queue(&ready_queue);
            printf("\n");
            current_time++;
            idle_time++;

        } else {
            Process current_process = ready_queue.data[ready_queue.front];

            start_time = current_time;
            printf("%d\tStarted P%s\t", current_time, current_process.id);
            display_queue(&ready_queue);
            printf("\n");

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

            printf("%d\tCompleted P%s\t", current_time, current_process.id);
            display_queue(&ready_queue);
            printf("\n");

        }
    }

    printf("---------------------------------------\n");
    printf("Simulation complete.\n\n");

    qsort(processes, num_processes, sizeof(Process), compare_completion);

    display_metrics(processes, num_processes, idle_time, current_time);

    display_chart(processes, num_processes);

}

/* TODO */
void simulate_SJF_to_stdout(Process processes[], int num_processes) {

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
            display_queue(&ready_queue);
            printf("\n");
            current_time++;
            idle_time++;
        } else {

            sort_queue(&ready_queue, compare_burst);

            Process current_process = ready_queue.data[ready_queue.front];

            start_time = current_time;
            printf("%d\tStarted P%s\t", current_time, current_process.id);
            display_queue(&ready_queue);
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
            display_queue(&ready_queue);
            printf("\n");
        }
    }

    printf("---------------------------------------\n");
    printf("Simulation complete.\n\n");

    qsort(processes, num_processes, sizeof(Process), compare_completion);

    display_metrics(processes, num_processes, idle_time, current_time);

    display_chart(processes, num_processes);
}


void simulate_SJF_to_file(Process processes[], int num_processes, Options *options) {

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
            display_queue(&ready_queue);
            printf("\n");
            current_time++;
            idle_time++;
        } else {

            sort_queue(&ready_queue, compare_burst);

            Process current_process = ready_queue.data[ready_queue.front];

            start_time = current_time;
            printf("%d\tStarted P%s\t", current_time, current_process.id);
            display_queue(&ready_queue);
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
            display_queue(&ready_queue);
            printf("\n");
        }
    }

    printf("---------------------------------------\n");
    printf("Simulation complete.\n\n");

    qsort(processes, num_processes, sizeof(Process), compare_completion);

    display_metrics(processes, num_processes, idle_time, current_time);

    display_chart(processes, num_processes);

}

void simulate_priority_to_stdout(Process processes[], int num_processes) {

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
            display_queue(&ready_queue);
            printf("\n");
            current_time++;
            idle_time++;
        } else {

            sort_queue(&ready_queue, compare_priority);

            Process current_process = ready_queue.data[ready_queue.front];

            start_time = current_time;
            printf("%d\tStarted P%s\t", current_time, current_process.id);
            display_queue(&ready_queue);
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
            display_queue(&ready_queue);
            printf("\n");
        }
    }

    printf("---------------------------------------\n");
    printf("Simulation complete.\n\n");

    qsort(processes, num_processes, sizeof(Process), compare_completion);

    display_metrics(processes, num_processes, idle_time, current_time);

    display_chart(processes, num_processes);
}

/* TODO */
void simulate_priority_to_file(Process processes[], int num_processes, Options *options) {
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
            display_queue(&ready_queue);
            printf("\n");
            current_time++;
            idle_time++;
        } else {

            sort_queue(&ready_queue, compare_priority);

            Process current_process = ready_queue.data[ready_queue.front];

            start_time = current_time;
            printf("%d\tStarted P%s\t", current_time, current_process.id);
            display_queue(&ready_queue);
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
            display_queue(&ready_queue);
            printf("\n");
        }
    }

    printf("---------------------------------------\n");
    printf("Simulation complete.\n\n");

    qsort(processes, num_processes, sizeof(Process), compare_completion);

    display_metrics(processes, num_processes, idle_time, current_time);

    display_chart(processes, num_processes);
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

void display_metrics(Process processes[], int num_processes, int idle_time, int current_time) {
    int total_waiting = 0;
    int total_turnaround = 0;
    int total_response = 0; 
    int total_completion = current_time;

    for (int i = 0; i < num_processes; i++) {
        printf("Metrics for P%s: Turnaround= %d, Waiting= %d, Response= %d\n",
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

    printf("\nMetrics Summary:\n");
    printf("Average Waiting Time: %.2f\n", avg_waiting);
    printf("Average Turnaround Time: %.2f\n", avg_turnaround);
    printf("Average Response Time: %.2f\n", avg_response);
    printf("Throughput: %.2f processes per time unit\n", throughput);
    printf("CPU Utilization: %.2f%%\n", cpu_utilization);
}


void display_chart(Process processes[], int num_processes) {

    int max_time = 0;
    int current_time = 0;

    for (int i = 0; i < num_processes; i++) {
        if (processes[i].completion_time > max_time) {
            max_time = processes[i].completion_time;
        }
    }

    printf("\nGantt Chart:\n");
    printf(ANSI_BOLD ANSI_BLUE "--------------------------------------------" ANSI_RESET "\n");

    for (int start = 0; start <= max_time; start += MAX_WIDTH) {

        int end = (start + MAX_WIDTH - 1 < max_time) ? start + MAX_WIDTH - 1 : max_time;

        
        printf(ANSI_BOLD ANSI_BLUE "Time: " ANSI_RESET);
        for (int t = start; t <= end; t++) {
            printf(ANSI_BOLD ANSI_BLUE "%-4d" ANSI_RESET, t);  
        }
        printf("\n");

        
        for (int i = 0; i < num_processes; i++) {
            const char* color;
            switch (i % 5) {
                case 0: color = ANSI_RED; break;
                case 1: color = ANSI_GREEN; break;
                case 2: color = ANSI_YELLOW; break;
                case 3: color = ANSI_BLUE; break;
                case 4: color = ANSI_MAGENTA; break;
            }
            printf("%sP%-2s| " ANSI_RESET, color, processes[i].id);  

            for (int j = start; j <= end; j++) {
                if (j >= processes[i].start_time && j <= processes[i].completion_time) {
                    
                    printf("%s### " ANSI_RESET, color); 
                    
                } else {
                    printf("    "); 
                }
                current_time++;
            }
            printf("\n");
        }

        printf(ANSI_BOLD ANSI_BLUE "--------------------------------------------" ANSI_RESET "\n");
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

void simulate_SRTF(Process processes[], int num_processes, float alpha, int default_burst) {

    CircularQueue ready_queue;
    init_queue(&ready_queue);

    int current_time = 0;
    int idle_time = 0;
    int start_time;
    int preempted = 0;
    char last_process_id[10] = "";

    // Initialize predicted burst times
    for (int j = 0; j < num_processes; j++) {
        processes[j].predicted_burst = default_burst;
    }

    // Sort processes by arrival time
    qsort(processes, num_processes, sizeof(Process), compare_arrival);

    printf("\nRunning Simulation for Preemptive SJF (Shortest Job First, with Predictions)\n\n");
    printf("\n\nTime\tEvent\t\tReady Queue\n");
    printf("---------------------------------------\n");

    int i = 0;
    while (i < num_processes || !isEmpty(&ready_queue)) {
        // Add processes that have arrived
        while (i < num_processes && processes[i].arrival_time <= current_time) {
            enqueue(&ready_queue, processes[i]);
            i++;
        }

        if (isEmpty(&ready_queue)) {
            printf("%d\tIdle\t\t", current_time);
            display_queue(&ready_queue);
            printf("\n");
            current_time++;
            idle_time++;
        } else {
            
            sort_queue(&ready_queue, compare_predicted_burst);

            Process *current_process = &ready_queue.data[ready_queue.front];

            if (strcmp(current_process->id, last_process_id) != 0) {
                preempted = 1;
            }


            if (current_process->remaining_time == current_process->burst_time) {
                current_process->start_time = current_time;
            }

            printf("%d\tStarted P%s\t", current_time, current_process->id);
            display_queue(&ready_queue);
            printf("\n");

        
            current_process->remaining_time--;
            current_time++;

            
            if (current_process->remaining_time == 0) {
                dequeue(&ready_queue);
                printf("%d\tCompleted P%s\t", current_time, current_process->id);
                display_queue(&ready_queue);
                printf("\n");

                
                for (int j = 0; j < num_processes; j++) {
                    if (strcmp(processes[j].id, current_process->id) == 0) {
                        processes[j].completion_time = current_time;
                        processes[j].turnaround_time = current_time - processes[j].arrival_time;
                        processes[j].waiting_time = processes[j].turnaround_time - processes[j].burst_time;
                        processes[j].response_time = start_time - processes[j].arrival_time;
                        break;
                    }
                }
            }

    
            if (preempted) {

                Process *previous_process = NULL;
                for (int j = 0; j < ready_queue.fill; j++) {
                    if (strcmp(ready_queue.data[j].id, last_process_id) == 0) {
                        previous_process = &ready_queue.data[j];
                        break;
                    }
                }

                if (previous_process) {
                    float execution_time = previous_process->burst_time - previous_process->remaining_time;
                    previous_process->predicted_burst = 
                        alpha * execution_time + (1 - alpha) * previous_process->predicted_burst;
                }

                preempted = 0;
            }

            strncpy(last_process_id, current_process->id, sizeof(last_process_id));

        
            while (i < num_processes && processes[i].arrival_time <= current_time) {
                enqueue(&ready_queue, processes[i]);
                i++;
            }
        }
    }

    printf("---------------------------------------\n");
    printf("Simulation complete.\n\n");

    qsort(processes, num_processes, sizeof(Process), compare_completion);
    display_metrics(processes, num_processes, idle_time, current_time);
    display_chart(processes, num_processes);
}



void simulate_preemptive_priority(Process processes[], int num_processes) {

    CircularQueue ready_queue;
    init_queue(&ready_queue);

    int current_time = 0;
    int idle_time = 0;
    int start_time;

    qsort(processes, num_processes, sizeof(Process), compare_arrival);

    printf("\nRunning Simulation for Preemptive SJF (Shortest Job First, with Predictions)\n\n");
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
            display_queue(&ready_queue);
            printf("\n");
            current_time++;
            idle_time++;
        } else {
            
            sort_queue(&ready_queue, compare_priority);

            Process *current_process = &ready_queue.data[ready_queue.front];


            if (current_process->remaining_time == current_process->burst_time) {
                current_process->start_time = current_time;
            }

            printf("%d\tStarted P%s\t", current_time, current_process->id);
            display_queue(&ready_queue);
            printf("\n");

        
            current_process->remaining_time--;
            current_time++;

            
            if (current_process->remaining_time == 0) {
                dequeue(&ready_queue);
                printf("%d\tCompleted P%s\t", current_time, current_process->id);
                display_queue(&ready_queue);
                printf("\n");

                
                for (int j = 0; j < num_processes; j++) {
                    if (strcmp(processes[j].id, current_process->id) == 0) {
                        processes[j].completion_time = current_time;
                        processes[j].turnaround_time = current_time - processes[j].arrival_time;
                        processes[j].waiting_time = processes[j].turnaround_time - processes[j].burst_time;
                        processes[j].response_time = start_time - processes[j].arrival_time;
                        break;
                    }
                }
            }
       
            while (i < num_processes && processes[i].arrival_time <= current_time) {
                enqueue(&ready_queue, processes[i]);
                i++;
            }
        }
    }

    printf("---------------------------------------\n");
    printf("Simulation complete.\n\n");

    qsort(processes, num_processes, sizeof(Process), compare_completion);
    display_metrics(processes, num_processes, idle_time, current_time);
    display_chart(processes, num_processes);
}

void simulate_round_robin(Process processes[], int num_processes, int time_quantum) {

    CircularQueue ready_queue;
    init_queue(&ready_queue);

    int current_time = 0;
    int idle_time = 0;
    int i = 0;

    printf("\nRunning Simulation for Round Robin Scheduling\n\n");
    qsort(processes, num_processes, sizeof(Process), compare_arrival); 

    printf("\n\nTime\tEvent\t\tReady Queue\n");
    printf("---------------------------------------\n");

    while (i < num_processes || !isEmpty(&ready_queue)) {

        
        while (i < num_processes && processes[i].arrival_time <= current_time) {
            enqueue(&ready_queue, processes[i]);
            i++;
        }

        if (isEmpty(&ready_queue)) {
            
            printf("%d\tIdle\t\t", current_time);
            display_queue(&ready_queue);
            printf("\n");
            current_time++;
            idle_time++;
        } else {
            
            Process current_process = ready_queue.data[ready_queue.front];

            
            for (int j = 0; j < num_processes; j++) {
                if (strcmp(processes[j].id, current_process.id) == 0 && !processes[j].has_started) {
                    processes[j].response_time = current_time - processes[j].arrival_time;
                    processes[j].start_time = current_time;
                    processes[j].has_started = 1;
                    break;
                }
            }

            
            int exec_time = (current_process.remaining_time > time_quantum) ? time_quantum : current_process.remaining_time;
            printf("%d\tRunning P%s\t", current_time, current_process.id);
            display_queue(&ready_queue);
            printf("\n");
            dequeue(&ready_queue);

            current_time += exec_time;
            current_process.remaining_time -= exec_time;

            
            while (i < num_processes && processes[i].arrival_time <= current_time) {
                enqueue(&ready_queue, processes[i]);
                i++;
            }

            
            if (current_process.remaining_time > 0) {
                enqueue(&ready_queue, current_process); 
            } else {
                
                for (int j = 0; j < num_processes; j++) {
                    if (strcmp(processes[j].id, current_process.id) == 0) {
                        processes[j].completion_time = current_time;
                        processes[j].turnaround_time = current_time - processes[j].arrival_time;
                        processes[j].waiting_time = processes[j].turnaround_time - processes[j].burst_time;
                        break;
                    }
                }
                printf("%d\tCompleted P%s\t", current_time, current_process.id);
                display_queue(&ready_queue);
                printf("\n");
            }
        }
    }

    printf("---------------------------------------\n");
    printf("Simulation complete.\n\n");


    display_metrics(processes, num_processes, idle_time, current_time);
    display_chart(processes, num_processes);
}

int compare_predicted_burst(const void *a, const void *b) {
    Process *process_a = (Process *)a;
    Process *process_b = (Process *)b;

    if (process_a->predicted_burst != process_b->predicted_burst) {
        return process_a->predicted_burst - process_b->predicted_burst;
    }

    /*If Burst times equal compare by arrival*/
    return process_a->arrival_time - process_b->arrival_time;
}