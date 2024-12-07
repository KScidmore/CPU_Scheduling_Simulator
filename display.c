/*---------- ID HEADER -------------------------------------
/  Author(s):   Andrew Boisvert, Kyle Scidmore
/  Email(s):    abois526@mtroyal.ca, kscid125@mtroyal.ca 
/  File Name:   display.c
/
/  File Description:
/    This file implements functions for displaying process
/    scheduling metrics and charts. These functions are used
/    to visualize scheduling information, including Gantt charts
/    and related metrics.
/
/---------------------------------------------------------*/
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

/*---------- FUNCTION: display_metrics --------------------------------
/  Function Description:
/    Calculates and displays scheduling metrics for a set of processes,
/    including average waiting time, turnaround time, response time, 
/    CPU utilization, and throughput. Outputs detailed per-process 
/    metrics and overall metrics to a file or standard output.
/  
/  Caller Input:
/    - Process processes[]: Array of processes with completed metrics
/    - int num_processes: Total number of processes in the array.
/    - int idle_time: Total idle time of the CPU during the simulation.
/    - int current_time: Total time elapsed in the simulation (completion time).
/    - FILE *fp: File pointer for writing metrics (can be stdout if no file).
/  
/  Caller Output:
/    N/A - No return value. Results are written to the provided file or 
/          standard output.
/  
/  Assumptions, Limitations, Known Bugs:
/    -
/
------------------------------------------------------------------------*/
void display_metrics(Process processes[], int num_processes, int idle_time, int current_time) {
    int total_waiting = 0;
    int total_turnaround = 0;
    int total_response = 0; 
    int total_completion = current_time;

    for (int i = 0; i < num_processes; i++) {
        fprintf(stdout, "Metrics for P%s: Turnaround= %d, Waiting= %d, Response= %d\n",
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

    fprintf(stdout, "\nMetrics Summary:\n");
    fprintf(stdout, "Average Waiting Time: %.2f\n", avg_waiting);
    fprintf(stdout, "Average Turnaround Time: %.2f\n", avg_turnaround);
    fprintf(stdout, "Average Response Time: %.2f\n", avg_response);
    fprintf(stdout, "Throughput: %.2f processes per time unit\n", throughput);
    fprintf(stdout, "CPU Utilization: %.2f%%\n", cpu_utilization);
}

void display_metrics_table(Process processes[], int num_processes, int idle_time, int current_time) {
    int total_waiting = 0;
    int total_turnaround = 0;
    int total_response = 0; 
    int total_completion = current_time;
    char pid[11];

    fprintf(stdout, "Table of Metrics Values:");
    fprintf(stdout, "\n+-------+-------+-------+-------+\n");
    fprintf(stdout, "|  PID  | Turn. | Wait. | Resp. |");
    fprintf(stdout, "\n+-------+-------+-------+-------+\n");
    for (int i = 0; i < num_processes; i++) {
        sprintf(pid, "P%s", processes[i].id);
        fprintf(stdout, "|  %3s  |   %2d  |   %2d  |   %2d  |\n",
                pid,
                processes[i].turnaround_time,
                processes[i].waiting_time,
                processes[i].response_time);
        fprintf(stdout, "+-------+-------+-------+-------+\n");
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

    fprintf(stdout, "\nMetrics Summary:\n");
    fprintf(stdout, "Average Waiting Time:\n\t%.2f\n", avg_waiting);
    fprintf(stdout, "Average Turnaround Time:\n\t%.2f\n", avg_turnaround);
    fprintf(stdout, "Average Response Time:\n\t%.2f\n", avg_response);
    fprintf(stdout, "Throughput:\n\t%.2f processes per time unit\n", throughput);
    fprintf(stdout, "CPU Utilization:\n\t%.2f%%\n", cpu_utilization);
}


/*---------- FUNCTION DOCUMENTATION BLOCK ------------------------------
/  Functions Overview:
/    These functions generate a Gantt chart representation of process scheduling,
/    visually displaying the start and completion times of each process in a 
/    time-ordered format.
/  
/  Functions and Their Purpose:
/    - display_chart: Generates a Gantt chart with color-coded output using ANSI 
/      escape codes, intended for terminal display.
/    - display_chart_file: Generates a plain-text Gantt chart without ANSI codes, 
/      suitable for writing to output files.
/    - display_preemptive_chart: Generates a Gantt chart for preemptive scheduling 
/      scenarios, where specific time slices need to be tracked.
/  
/  Input Parameters:
/    - Process processes[]: Array of processes to display in the chart. 
/    - int num_processes: The number of processes in the array.
/    - FILE *fp: A file pointer where the Gantt chart is written.
/  
/  Output:
/    - display_chart and display_chart_file write formatted Gantt chart output 
/      to the correct output based on file pointer
/    - display_preemptive_chart outputs the Gantt chart to the console, formatted 
/       for preemptive scheduling.
/  
/  Assumptions, Limitations, Known Bugs:
/    - No known bugs at this time.
/  
/------------------------------------------------------------------------*/
void display_chart(Process processes[], int num_processes) {
   
    int max_time = 0;
    int current_time = 0;

    for (int i = 0; i < num_processes; i++) {
        if (processes[i].completion_time > max_time) {
            max_time = processes[i].completion_time;
        }
    }

    fprintf(stdout, "\nGantt Chart:\n");
    fprintf(stdout, ANSI_BOLD ANSI_BLUE "--------------------------------------------" ANSI_RESET "\n");

    for (int start = 0; start <= max_time; start += MAX_WIDTH) {

        int end = (start + MAX_WIDTH - 1 < max_time) ? start + MAX_WIDTH - 1 : max_time;

        
        fprintf(stdout, ANSI_BOLD ANSI_BLUE "Time: " ANSI_RESET);
        for (int t = start; t <= end; t++) {
            fprintf(stdout, ANSI_BOLD ANSI_BLUE "%-4d" ANSI_RESET, t);  
        }
        fprintf(stdout, "\n");

        
        for (int i = 0; i < num_processes; i++) {
            const char* color;
            switch (i % 5) {
                case 0: color = ANSI_RED; break;
                case 1: color = ANSI_GREEN; break;
                case 2: color = ANSI_YELLOW; break;
                case 3: color = ANSI_BLUE; break;
                case 4: color = ANSI_MAGENTA; break;
            }
            fprintf(stdout, "%sP%-2s| " ANSI_RESET, color, processes[i].id);  

            for (int j = start; j <= end; j++) {
                if (j >= processes[i].start_time && j <= processes[i].completion_time) {
                    
                    fprintf(stdout, "%s### " ANSI_RESET, color); 
                    
                } else {
                    fprintf(stdout, "    "); 
                }
                current_time++;
            }
            fprintf(stdout, "\n");
        }

        fprintf(stdout, ANSI_BOLD ANSI_BLUE "--------------------------------------------" ANSI_RESET "\n");
    }
}

void display_chart_file(Process processes[], int num_processes) {
   
    int max_time = 0;
    int current_time = 0;

    for (int i = 0; i < num_processes; i++) {
        if (processes[i].completion_time > max_time) {
            max_time = processes[i].completion_time;
        }
    }

    fprintf(stdout, "\nGantt Chart:\n");
    fprintf(stdout, "--------------------------------------------\n");

    for (int start = 0; start <= max_time; start += MAX_WIDTH) {

        int end = (start + MAX_WIDTH - 1 < max_time) ? start + MAX_WIDTH - 1 : max_time;

        
        fprintf(stdout, "Time: ");
        for (int t = start; t <= end; t++) {
            fprintf(stdout, "%-4d", t);  
        }
        fprintf(stdout, "\n");

        
        for (int i = 0; i < num_processes; i++) {
            fprintf(stdout, "P%-2s| ", processes[i].id);  

            for (int j = start; j <= end; j++) {
                if (j >= processes[i].start_time && j <= processes[i].completion_time) {
                    
                    fprintf(stdout, "### "); 
                    
                } else {
                    fprintf(stdout, "    "); 
                }
                current_time++;
            }
            fprintf(stdout, "\n");
        }

        fprintf(stdout, "--------------------------------------------\n");
    }
}

void display_preemptive_chart(Process processes[], int num_processes) {
    int max_time = 0;
    int current_time = 0;
    int flag = 0;

    
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

            
            for (int t = start; t <= end; t++) {
                if (processes[i].running_time[t] == 1) {  
                    printf("%s### " ANSI_RESET, color); 
                } else {
                    if(!flag){

                    }

                    printf("    ");  
                }
                current_time++;
            }
            printf("\n");
        }

        printf(ANSI_BOLD ANSI_BLUE "--------------------------------------------" ANSI_RESET "\n");
    }
}


