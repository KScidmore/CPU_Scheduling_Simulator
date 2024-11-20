#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sim.h"
#include "process.h"
#include "scheduling.h"
#include "globals.h"

int main(int argc, char *argv[]) {

    Process processes[MAX_PROCESSES];
    int choice;
    int time_quantum;

    int num_processes = 0;

    if(argc >= 3){

        /*Handle file data and run simulation for selected algo*/


    }else {

        /*Prompt user for algo and process details*/
        printf("\nSelect a Scheduling Algorithm:\n");
        printf("1. FCFS (First Come First Serve)\n");
        printf("2. SJF (Shortest Job First)\n");
        printf("3. RR (Round Robin)\n");
        printf("4. Priority Scheduling\n");
        printf("\nEnter your choice (1-4): ");
        scanf("%d", &choice);

        num_processes = process_input(processes);

        /*for (int i = 0; i < num_processes; i++) {
        printf("Process ID: %s\n", processes[i].id);
        printf("Arrival Time: %d\n", processes[i].arrival_time);
        printf("Burst Time: %d\n", processes[i].burst_time);*/


        switch(choice){
            case 1:
                /*Run FCFS*/
                simulate_FCFS(processes, num_processes);
                break;
            case 2:
                /*Run SJF*/
                break;
            case 3:
                /*Ask for time quantum and run RR*/
                printf("Enter Time Quantum: ");
                scanf("%d", &time_quantum);
                break;
            case 4:
                /*Run Priority*/
                break;
            default:
                printf("Invalid Choice\n");
                break;
        }
    }
    
    return 0;
}

int process_input(Process processes[]){

    int num;
    printf("\nEnter the number of processes (max %d): ", MAX_PROCESSES);
    scanf("%d", &num);

    if(num < 1 || num > MAX_PROCESSES){
        printf("Invalid number of processes. Exiting.\n");
        exit(1);
    }

    for(int i = 0; i < num; i++){

        printf("\nEnter Process Details %d\n", i + 1);
        printf("Process ID: ");
        scanf("%s", processes[i].id);
        printf("Arrival Time: ");
        scanf("%d", &processes[i].arrival_time);
        printf("Burst Time: ");
        scanf("%d", &processes[i].burst_time);

        /*Set up other default values*/
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].start_time = -1;
        processes[i].completion_time = 0;
        processes[i].turnaround_time = 0;
        processes[i].waiting_time = 0;
        processes[i].response_time = -1;

    }

    return num;


}