/*Using qsort: https://www.tutorialspoint.com/c_standard_library/c_function_qsort.htm*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "process.h"
#include "scheduling.h"
#include "globals.h"
#include "circular_queue.h"
#include "ansi_esc_seqs.h"
#include "options.h"
#include "display.h"
#include "compare.h"


/*---------- FUNCTION: simulate_FCFS -----------------------------------
/  Function Description:
/    Simulates the First Come First Served scheduling algorithm on a set of 
/    processes. The function tracks process execution, waiting times, 
/    response times, and completion times. It outputs detailed logs to 
/    the console or an optional file stream, displaying scheduling events 
/    and overall metrics upon completion.
/  
/  Caller Input:
/    - Process processes[]: Array of processes to be scheduled.
/    - int num_processes: Number of processes in the array.
/    - FILE *fp: Optional file pointer for writing simulation logs 
/  
/  Caller Output:
/    N/A - No return value. Results are displayed directly in the console 
/          or written to the file if provided.
/  
/  Assumptions, Limitations, Known Bugs:
/    - 
/
-------------------------------------------------------------------------*/
void simulate_FCFS(Process processes[], int num_processes, Options *options) {

    CircularQueue ready_queue;
    init_queue(&ready_queue);

    int current_time = 0;
    int idle_time = 0;
    int start_time;

    qsort(processes, num_processes, sizeof(Process), compare_arrival);

    fprintf(stdout, "~~~~~~~~~~\n\n");
    //fprintf(fp, "\n\nTime\tEvent\t\tReady Queue\n");
    //fprintf(fp, "\n\n+---------+---------------------+----------------------------------\n");
    fprintf(stdout, "+---------+---------------------+---------------------------\n");
    fprintf(stdout, "| %-8s| %-20s| %-40s", "Time", "Event", "Ready Queue");
    fprintf(stdout, "\n+---------+---------------------+---------------------------\n");
    //fprintf(fp, "\n---------------------------------------\n");

    int i = 0; 
    while (i < num_processes || !isEmpty(&ready_queue)) {
       
        /*Check if processes have arrived, stops when it finds a process that hasnt*/
        while (i < num_processes && processes[i].arrival_time <= current_time) {
            enqueue(&ready_queue, processes[i]);
            i++;
        }

        /*If queue is empty track idle state */
        if (isEmpty(&ready_queue)) {

            //fprintf(fp, "%d\tIdle\t\t", current_time);
            fprintf(stdout, "| %-8d| %-20s| ", current_time, "Idle");

            display_queue(&ready_queue);
            printf("\n");
            while(isEmpty(&ready_queue)){
                current_time++;
                idle_time++;

                while (i < num_processes && processes[i].arrival_time <= current_time) {
                    enqueue(&ready_queue, processes[i]);
                    i++;
                }

            }

        } else {
            Process current_process = ready_queue.data[ready_queue.front];

            start_time = current_time;
            //fprintf(fp, "%d\tStarted P%s\t", current_time, current_process.id);
            fprintf(stdout, "| %-8d| Started P%-11s| ", current_time, current_process.id);
            display_queue(&ready_queue);
            fprintf(stdout, "\n");

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

            //fprintf(fp, "%d\tCompleted P%s\t", current_time, current_process.id);
            fprintf(stdout, "| %-8d| Completed P%-9s| ", current_time, current_process.id);
            display_queue(&ready_queue);
            fprintf(stdout, "\n");

        }
    }

    //printf("---------------------------------------\n");
    fprintf(stdout, "+---------+---------------------+---------------------------\n");
    printf("Simulation complete.");
    fprintf(stdout, "\n\n~~~~~~~~~~\n\n");

    qsort(processes, num_processes, sizeof(Process), compare_completion);

    display_metrics(processes, num_processes, idle_time, current_time);
    fprintf(stdout, "\n~~~~~~~~~~\n");

    isatty(fileno(stdout)) ?
    display_chart(processes, num_processes) :
    display_chart_file(processes, num_processes);
    fprintf(stdout, "\n~~~~~~~~~~\n");

}

/*---------- FUNCTION: simulate_SJF -----------------------------------
/  Function Description:
/    Simulates the Shortest Job First scheduling algorithm on a set of 
/    processes. The function tracks process execution, waiting times, 
/    response times, and completion times. It outputs detailed logs to 
/    the console or an optional file stream, displaying scheduling events 
/    and overall metrics upon completion.
/  
/  Caller Input:
/    - Process processes[]: Array of processes to be scheduled.
/    - int num_processes: Number of processes in the array.
/    - FILE *fp: Optional file pointer for writing simulation logs 
/  
/  Caller Output:
/    N/A - No return value. Results are displayed directly in the console 
/          or written to the file if provided.
/  
/  Assumptions, Limitations, Known Bugs:
/    - 
/
-------------------------------------------------------------------------*/
void simulate_SJF(Process processes[], int num_processes, Options *options) {


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
            while(isEmpty(&ready_queue)){
                current_time++;
                idle_time++;

                while (i < num_processes && processes[i].arrival_time <= current_time) {
                    enqueue(&ready_queue, processes[i]);
                    i++;
                }

            }
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

/*---------- FUNCTION: simulate_priority -------------------------------
/  Function Description:
/    Simulates the priority scheduling algorithm on a set of processes.
/    The function tracks process execution, waiting times, response times,
/    and completion times. It outputs detailed logs to the console or an
/    optional file stream, displaying scheduling events and overall metrics 
/    upon completion.
/  
/  Caller Input:
/    - Process processes[]: Array of processes to be scheduled.
/    - int num_processes: Number of processes in the array.
/    - FILE *fp: Optional file pointer for writing simulation logs 
/  
/  Caller Output:
/    N/A - No return value. Results are displayed directly in the console 
/          or written to the file if provided.
/  
/  Assumptions, Limitations, Known Bugs:
/    - 
/
------------------------------------------------------------------------*/
void simulate_priority(Process processes[], int num_processes, Options *options) {
    
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
            display_queue(&ready_queue);
            printf("\n");
            while(isEmpty(&ready_queue)){
                current_time++;
                idle_time++;

                while (i < num_processes && processes[i].arrival_time <= current_time) {
                    enqueue(&ready_queue, processes[i]);
                    i++;
                }

            }
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

/*---------- FUNCTION: simulate_SRTF -----------------------------------
/  Function Description:
/    Simulates the Shortest Remaining Time First scheduling algorithm 
/    on a set of processes. The function tracks process execution, 
/    waiting times, response times, and completion times. It outputs 
/    detailed logs to the console or an optional file stream, 
/    displaying scheduling events and overall metrics upon completion.
/  
/  Caller Input:
/    - Process processes[]: Array of processes to be scheduled.
/    - int num_processes: Number of processes in the array.
/    - int time_quantum: Time slice for Round Robin scheduling.
/    - FILE *fp: Optional file pointer for writing simulation logs 
/  
/  Caller Output:
/    N/A - No return value. Results are displayed directly in the console 
/          or written to the file if provided.
/  
/  Assumptions, Limitations, Known Bugs:
/    - 
/
-------------------------------------------------------------------------*/
void simulate_SRTF(Process processes[], int num_processes) {

    CircularQueue ready_queue;
    init_queue(&ready_queue);

    int current_time = 0;
    int idle_time = 0;
    char last_process_id[10] = "";
    int idled = 0;

    
    qsort(processes, num_processes, sizeof(Process), compare_arrival);

    printf("\nRunning Simulation for SRTF (Preemptive SJF)\n\n");
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
            while(isEmpty(&ready_queue)){
                current_time++;
                idle_time++;

                while (i < num_processes && processes[i].arrival_time <= current_time) {
                    enqueue(&ready_queue, processes[i]);
                    i++;
                }

            }
            idled = 1;
        } else {
            
            sort_queue(&ready_queue, compare_burst);

            Process *current_process = &ready_queue.data[ready_queue.front];

            if (strcmp(last_process_id, current_process->id) != 0) {
                printf("%d\tStarted P%s\t", current_time, current_process->id);
                display_queue(&ready_queue);
                printf("\n");

                if(!idled){

                    for (int j = 0; j < num_processes; j++) {

                        if (strcmp(processes[j].id, last_process_id) == 0) {
                            processes[j].running_time[current_time] = 1;
                            break;
                        }
                    }

                }

                strcpy(last_process_id, current_process->id);
                
                if (current_process->remaining_time == current_process->burst_time) {
                    current_process->start_time = current_time;
                }
            }

            for (int j = 0; j < num_processes; j++) {

                if (strcmp(processes[j].id, current_process->id) == 0) {
                    processes[j].running_time[current_time] = 1;
                    break;
                }
            }
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
                        processes[j].response_time = current_process->start_time - processes[j].arrival_time;
                        processes[j].running_time[current_time] = 1;
                        break;
                    }
                }
            }

            idled = 0;

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
    qsort(processes, num_processes, sizeof(Process), compare_arrival);
    display_preemptive_chart(processes, num_processes);
}


/*---------- FUNCTION: simulate_preemptive_priority ----------------------------
/  Function Description:
/    Simulates the Preemptive Priority scheduling algorithm on a set of processes.
/    The function tracks process execution, waiting times, response times,
/    and completion times. It outputs detailed logs to the console or an
/    optional file stream, displaying scheduling events and overall metrics upon 
/    completion.
/  
/  Caller Input:
/    - Process processes[]: Array of processes to be scheduled.
/    - int num_processes: Number of processes in the array.
/    - int time_quantum: Time slice for Round Robin scheduling.
/    - FILE *fp: Optional file pointer for writing simulation logs 
/  
/  Caller Output:
/    N/A - No return value. Results are displayed directly in the console 
/          or written to the file if provided.
/  
/  Assumptions, Limitations, Known Bugs:
/    - 
/
---------------------------------------------------------------------------------*/
void simulate_preemptive_priority(Process processes[], int num_processes) {

    CircularQueue ready_queue;
    init_queue(&ready_queue);

    int current_time = 0;
    int idle_time = 0;
    int start_time;
    char last_process_id[10] = "";
    int idled = 0;
    int first_run = 1;

    qsort(processes, num_processes, sizeof(Process), compare_arrival);

    printf("\nRunning Simulation for Preemptive Priority\n\n");
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
            while(isEmpty(&ready_queue)){
                current_time++;
                idle_time++;

                while (i < num_processes && processes[i].arrival_time <= current_time) {
                    enqueue(&ready_queue, processes[i]);
                    i++;
                }

            }
            idled = 1;
        } else {
            
            sort_queue(&ready_queue, compare_priority);

            Process *current_process = &ready_queue.data[ready_queue.front];


            if (strcmp(last_process_id, current_process->id) != 0) {
                
                printf("%d\tStarted P%s\t", current_time, current_process->id);
                display_queue(&ready_queue);
                printf("\n");

                if(!idled){

                    for (int j = 0; j < num_processes; j++) {

                        if (strcmp(processes[j].id, last_process_id) == 0) {
                            processes[j].running_time[current_time] = 1;
                            break;
                        }
                    }

                }

                strcpy(last_process_id, current_process->id);
                
                if (current_process->remaining_time == current_process->burst_time) {
                    current_process->start_time = current_time;
                }
            }

            for (int j = 0; j < num_processes; j++) {

                if (strcmp(processes[j].id, current_process->id) == 0) {
                    processes[j].running_time[current_time] = 1;
                    break;
                }
            }
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
                        processes[j].response_time = current_process->start_time - processes[j].arrival_time;
                        processes[j].running_time[current_time] = 1;
                        break;
                    }
                }
            }

            idled = 0;

            Process *last_process = &ready_queue.data[ready_queue.front];

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
    qsort(processes, num_processes, sizeof(Process), compare_arrival);
    display_preemptive_chart(processes, num_processes);
}

/*---------- FUNCTION: simulate_round_robin ----------------------------
/  Function Description:
/    Simulates the Round Robin scheduling algorithm on a set of processes.
/    The function tracks process execution, waiting times, response times,
/    and completion times. It outputs detailed logs to the console or an
/    optional file stream, displaying scheduling events and overall metrics 
/    upon completion.
/  
/  Caller Input:
/    - Process processes[]: Array of processes to be scheduled.
/    - int num_processes: Number of processes in the array.
/    - int time_quantum: Time slice for Round Robin scheduling.
/    - FILE *fp: Optional file pointer for writing simulation logs 
/  
/  Caller Output:
/    N/A - No return value. Results are displayed directly in the console 
/          or written to the file if provided.
/  
/  Assumptions, Limitations, Known Bugs:
/    - 
/
------------------------------------------------------------------------*/
void simulate_round_robin(Process processes[], int num_processes, int time_quantum) {

    CircularQueue ready_queue;
    init_queue(&ready_queue);

    int current_time = 0;
    int idle_time = 0;
    int i = 0;
    int idling = 0;
    char last_process_id[10] = "";

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
            while(isEmpty(&ready_queue)){
                current_time++;
                idle_time++;

                while (i < num_processes && processes[i].arrival_time <= current_time) {
                    enqueue(&ready_queue, processes[i]);
                    i++;
                }

            }

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


            if(strcmp(current_process.id, last_process_id) != 0){

                printf("%d\tStarted P%s\t", current_time, current_process.id);
                display_queue(&ready_queue);
                printf("\n");
                strcpy(last_process_id, current_process.id);

            }

            dequeue(&ready_queue);

            for (int j = 0; j < num_processes; j++) {

                if (strcmp(processes[j].id, current_process.id) == 0) {
                    
                    int time_unit = current_time;
                    for(int k = 0; k <= exec_time; k++){

                        processes[j].running_time[time_unit] = 1;
                        time_unit++;

                    }
                    
                    break;
                }
            }

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
    display_preemptive_chart(processes, num_processes);
}