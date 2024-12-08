/*---------- ID HEADER -------------------------------------
/  Author(s):   Andrew Boisvert, Kyle Scidmore
/  Email(s):    abois526@mtroyal.ca, kscid125@mtroyal.ca 
/  File Name:   sim.c
/
/  Program Description:
/    Simulates various process scheduling algorithms, providing users
/    with visualizations and metrics to help them to understand and 
/    analayze the differences between the algorithms and each of their 
/    benefits/drawbacks. The program supports both interactive and 
/    command-line based inputs. The algorithms which the program supports 
/    are First Come First Serve, Shortest Job First, Shortest Remaining 
/    Time First, Round Robin, Priority, and Preemptive Priority. 
/
/  Program Details:
/    Program Input:
/      - Command-line Arguments:
/        -a <string>    where <string> = abbreviated algorithm selection
/        -i <string>    where <string> = full input filepath
/        -o <string>    where <string> = full output filepath
/        -h/--help      outputs a help menu 
/      - Interactive:
/        - algorithm selection, number of processes, and process details
/          are all entered by the user into terminal prompts
/      - Bash I/O Redirection:
/        - the program stil supports bash I/O redirection with < and > 
/    
/    Program Output:
/      - Contents:
/        - A table of scheduling results, showing the current time, current
/          event taking place, and state of the ready queue 
/        - A table of metrics values showing the turnaround, waiting time, 
/          and response time for each of the processes 
/        - A list of average waiting time, average turnaround time, average 
/          response time, throughput, and % of CPU utilization 
/        - A Gantt chart to help visualize the activity of the processes 
/          over the course of time
/      - Formatting:
/        - output of the results is printed to the terminal through stdout
/        - if the user specifies an output file, using arguments or bash I/O 
/          redirection, stdout will be redirected to that file, similarly 
/          following the manner that Bash performs this 
/        - Gantt charts are printed using ASCI escape sequence colour codes
/          when printing to the terminal. When printing to a file, this causes
/          garbage characters to be inserted into the file, so colour-coding
/          is not available. 
/    
/    Data Structures: 
/      - Process:
/        - represents an individual process with members for process details
/          and data needed to track metrics 
/      - CircularQueue:
/        - manages the ready queue for the process simulation algorithms
/      - Options:
/        - stores settings passed by command-line arguments 
/    
/    Misc:
/      - Assumes valid file formatting when passing an input file using 
/        either command-line arguments or bash I/O redirection operators
/      - Only handles up to a set value of processes, determined by the 
/        MAX_PROCESSES macro 
/---------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "sim.h"
#include "process.h"
#include "scheduling.h"
#include "globals.h"
#include "options.h"


int parse_cli_args(int argc, char **argv, Options *options);
void init_options(Options *options);
void print_help();
int process_input(Process processes[], int choice);
int process_file_input(Process processes[], Options *options, int choice);
void run_cli_mode(int argc, char **argv, Process processes[], Options *options);
void run_interactive_mode(Process processes[], Options *options);
void run_selected_algorithm(Process processes[], int num_processes, Options *options);
void print_scheduling_menu();
int terminal_prompt();
void str_to_upper(char *str);


int main(int argc, char **argv) {
    Process processes[MAX_PROCESSES];
    Options options;
    
    init_options(&options);
    parse_cli_args(argc, argv, &options);

    FILE *og_stdin = stdin;
    FILE *og_stdout = stdout;

    if (options.input_file[0] != '\0') {
        if (freopen(options.input_file, "r", stdin) == NULL) {
            fprintf(stderr, "Error: input file did not open.\n");
            exit(1);
        }
    }

    if (options.output_file[0] != '\0') {
        if (freopen(options.output_file, "w", stdout) == NULL) {
            fprintf(stderr, "Error: output file did not open.\n");
            exit(1);
        }
    }

    if (argc > 1) {
        run_cli_mode(argc, argv, processes, &options);
    } else {
        run_interactive_mode(processes, &options);
    }

    if (stdin != og_stdin) stdin = og_stdin;
    if (stdout != og_stdout) stdout = og_stdout;

    return 0;
    
}

/*---------- FUNCTION: run_cli_mode ------------------------
/  Function Description:
/    Handles the simulation when the user provides command-line 
/    arguments. Parses process data from a file, prompts the user
/    for info as necessary, then runs the selected algorithm. 
/  
/  Caller Input:
/    int argc: number of command-line args
/    char **argv: array of command-line args
/    Process processes[]: array of process details 
/    Options *options: pointer to parsed CLI input 
/  
/  Caller Output:
/    N/A - No output values
/  
/  Assumptions, Limitations, Known Bugs:
/    - errors are printed to stderr to avoid I/O redirection 
/    - does not gracefully handle invalid file input formats 
/    - this combo doesn't seem to work: ./sim < input.txt -o output.txt 
/---------------------------------------------------------*/
void run_cli_mode(int argc, char **argv, Process processes[], Options *options) {

    int num_processes = 0;
    int choice = 0;

    if (options->alg_selection[0] == '\0') {
        print_scheduling_menu();
        choice = terminal_prompt();
        switch (choice) {
            case 1: strcpy(options->alg_selection, "FCFS"); break;
            case 2: strcpy(options->alg_selection, "SJF"); break;
            case 3: strcpy(options->alg_selection, "SRTF"); break;
            case 4: strcpy(options->alg_selection, "RR"); break;
            case 5: strcpy(options->alg_selection, "PS"); break;
            case 6: strcpy(options->alg_selection, "PPS"); break;
            default: fprintf(stderr, "Invalid choice.\n"); break;
        } 
    } else {
        if (strcmp(options->alg_selection, "FCFS") == 0) {
            choice = 1;
        } else if (strcmp(options->alg_selection, "SJF") == 0) {
            choice = 2;
        } else if (strcmp(options->alg_selection, "SRTF") == 0){
            choice = 3;
        } else if (strcmp(options->alg_selection, "RR") == 0) {
            choice = 4;
        } else if (strcmp(options->alg_selection, "PS") == 0) {
            choice = 5;
        } else if (strcmp(options->alg_selection, "PPS") == 0) {
            choice = 6;
        } else{
            choice = 0;
            fprintf(stderr, "Invalid choice.\n");
        }
    }

    if (options->input_file[0] != '\0') {
        num_processes = process_file_input(processes, options, choice);
    } else {
        num_processes = process_input(processes, choice);
    }

    run_selected_algorithm(processes, num_processes, options);

}

/*---------- FUNCTION: terminal_prompt ---------------------
/  Function Description:
/    When input redirection is taking place and stdin is not directed
/    to the terminal, fscanf will not work correctly. This function works 
/    as a wrapper function, temporarily allowing the user to provide input 
/    to the terminal.
/  
/  Caller Input:
/    N/A - No input parameters
/  
/  Caller Output:
/    int choice: the user's menu selection 
/  
/  Assumptions, Limitations, Known Bugs:
/    N/A
/---------------------------------------------------------*/
int terminal_prompt() {
    int choice = 0;
    FILE *tty = fopen("/dev/tty", "r");
    if (stdout != tty) {
        fscanf(tty, "%d", &choice);
    }
    fclose(tty);
    return choice; 
}

/*---------- FUNCTION: run_interactive_mode ----------------
/  Function Description:
/    Handles the simulation when the user is going to interactively
/    provide details to the program. Allows the user to manually 
/    set the algorithm, process count, and process details. 
/  
/  Caller Input:
/    Process processes[]: array of process details 
/    Options *options: pointer to parsed CLI input 
/  
/  Caller Output:
/    N/A - No output values
/  
/  Assumptions, Limitations, Known Bugs:
/    N/A - all seems to be working as intended 
/---------------------------------------------------------*/
void run_interactive_mode(Process processes[], Options *options) {
    int choice;
    int num_processes;

    print_scheduling_menu();
    scanf("%d", &choice);

    switch (choice) {
        case 1: strcpy(options->alg_selection, "FCFS"); break;
        case 2: strcpy(options->alg_selection, "SJF"); break;
        case 3: strcpy(options->alg_selection, "SRTF"); break;
        case 4: strcpy(options->alg_selection, "RR"); break;
        case 5: strcpy(options->alg_selection, "PS"); break;
        case 6: strcpy(options->alg_selection, "PPS"); break;
        default: break;
    }

    num_processes = process_input(processes, choice);

    run_selected_algorithm(processes, num_processes, options);
}

/*---------- FUNCTION: print_scheduling_menu ---------------
/  Function Description:
/    Prints the menu that prompts the user to enter the 
/    type of scheduling algorithm they wish to simulate. 
/  
/  Caller Input:
/    N/A - No input parameters
/  
/  Caller Output:
/    N/A - No output values
/  
/  Assumptions, Limitations, Known Bugs:
/    - prints to stderr to ensure it prints to the terminal 
/      when stdin is redirected 
/---------------------------------------------------------*/
void print_scheduling_menu() {
    fprintf(stderr, "\nSelect a Scheduling Algorithm:\n");
    fprintf(stderr, "1. FCFS (First Come First Serve)\n");
    fprintf(stderr, "2. SJF (Shortest Job First)\n");
    fprintf(stderr, "3. SRTF (Shortest Remaining Time First)\n");
    fprintf(stderr, "4. RR (Round Robin)\n");
    fprintf(stderr, "5. PS (Priority Scheduling)\n");
    fprintf(stderr, "6. PPS (Preemptive Priority Scheduling)\n");
    fprintf(stderr, "\nEnter your choice (1-6): ");
}

/*---------- FUNCTION: run_selected_algorithm --------------
/  Function Description:
/    Runs the selected scheduling algorithm on the processes 
/    which have been provided. Prompts user for time quantum
/    if running RR. 
/  
/  Caller Input:
/    Process processes[]: array of process details 
/    int num_processes: the number of processes in the array 
/    Options *options: pointer to parsed CLI input 
/  
/  Caller Output:
/    N/A - No output values
/  
/  Assumptions, Limitations, Known Bugs:
/    - assumes processes[] contains valid data 
/    - assumes that a valid algorithm selection has taken place 
/---------------------------------------------------------*/
void run_selected_algorithm(Process processes[], int num_processes, Options *options) {
    if (strcmp(options->alg_selection, "FCFS") == 0) {
        simulate_FCFS(processes, num_processes, options);
    } else if (strcmp(options->alg_selection, "SJF") == 0) {
        simulate_SJF(processes, num_processes, options);
    } else if (strcmp(options->alg_selection, "RR") == 0) {
        int time_quantum;
        fprintf(stderr, "Please enter a time quantum for the simulation: ");
        time_quantum = terminal_prompt();
        simulate_round_robin(processes, num_processes, time_quantum);
    } else if (strcmp(options->alg_selection, "PS") == 0) {
        simulate_priority(processes, num_processes, options);
    } else if (strcmp(options->alg_selection, "SRTF") == 0){
        simulate_SRTF(processes, num_processes);
    } else if (strcmp(options->alg_selection, "PPS") == 0) {
        simulate_preemptive_priority(processes, num_processes);
    } else{
         printf("Invalid choice.\n");
    }
}

/*---------- FUNCTION: process_input -----------------------
/  Function Description:
/    Interactively collects the number of processes and the details
/    for each from the user. 
/  
/  Caller Input:
/    Process processes[]: array of process details 
/    int choice: the scheduling algorithm the user has selected
/  
/  Caller Output:
/    int num: the number of processes 
/  
/  Assumptions, Limitations, Known Bugs:
/    - prints to stderr to ensure it prints to the terminal 
/      when stdin is redirected 
/---------------------------------------------------------*/
int process_input(Process processes[], int choice) {
    fprintf(stderr, "%d", choice);
    int num;
    fprintf(stderr, "\nEnter the number of processes (max %d): ", MAX_PROCESSES);
    scanf("%d", &num);

    if(num < 1 || num > MAX_PROCESSES){
        fprintf(stderr, "Invalid number of processes. Exiting.\n");
        exit(1);
    }

    for (int i = 0; i < num; i++) {
        char temp_id[10];
        int unique = 0;

        fprintf(stderr, "\nEnter Process Details %d\n", i + 1);

        // Ensure the ID is unique
        while (!unique) {
            fprintf(stderr, "Process ID: ");
            scanf("%s", temp_id);

            if (is_unique_id(processes, i, temp_id)) {
                unique = 1; // ID is unique
                strcpy(processes[i].id, temp_id);
            } else {
                fprintf(stderr, "ID already taken. Please enter a unique ID.\n");
            }
        }

        fprintf(stderr, "Arrival Time: ");
        scanf("%d", &processes[i].arrival_time);
        fprintf(stderr, "Burst Time: ");
        scanf("%d", &processes[i].burst_time);


        if (choice == 5 || choice == 6) {
            fprintf(stderr, "Priority: ");
            scanf("%d", &processes[i].priority);
        }

        /* Set up other default values */
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].start_time = -1;
        processes[i].completion_time = 0;
        processes[i].turnaround_time = 0;
        processes[i].waiting_time = 0;
        processes[i].response_time = -1;
        processes[i].predicted_burst = -1;
        processes[i].has_started = 0;

        for(int j = 0; j < MAX_RUNTIME; j++){
            processes[i].running_time[j] = 0;
        }
    }

    fprintf(stderr, "\n");

    return num;
}

/*---------- FUNCTION: process_file_input ------------------
/  Function Description:
/    Reads process details from a file when specified by CLI args.
/    Depending on the algorithm, reads a differently formatted file
/    to ensure all necessary attributes are captured. 
/  
/  Caller Input:
/    Process processes[]: array of process details 
/    Options *options: pointer to parsed CLI input 
/    int choice: the scheduling algorithm the user has selected
/  
/  Caller Output:
/    int i: the number of processes in the file 
/  
/  Assumptions, Limitations, Known Bugs:
/    - assumes that the file format matches what is expected, errors 
/      might cause improper or incomplete population 
/    - prints to stderr to ensure it prints to the terminal 
/      when stdin is redirected 
/---------------------------------------------------------*/
int process_file_input(Process processes[], Options *options, int choice) {

    int i = 0;
    char temp_id[10];
    int unique = 0;
    char pid[10];
    int arrival, burst, priority, vals_read;

    if (options->input_file[0] == '\0') {
        fprintf(stderr, "Error: no input file.\n");
        return 0;
    }

    while (i < MAX_PROCESSES) {

        if (choice >= 0 && choice < 5) {
            vals_read = fscanf(stdin, "%s %d %d", temp_id, &arrival, &burst);
        } else if (choice == 5 || choice == 6) {
            vals_read = fscanf(stdin, "%s %d %d %d", temp_id, &arrival, &burst, &priority);
        } else {
            fprintf(stderr, "Error: invalid choice.\n");
        }

        if (vals_read == EOF) break;

        while (!unique) {
            if (is_unique_id(processes, i, temp_id)) {
                unique = 1;
                strcpy(processes[i].id, temp_id);
            } else {
                fprintf(stderr, "Error: ID is taken. Enter a unique ID.\n");
            }
        }
        unique = 0;

        processes[i].arrival_time = arrival;
        processes[i].burst_time = burst;
        processes[i].priority = (vals_read == 3) ? -1 : priority;
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].start_time = -1;         
        processes[i].completion_time = -1;    
        processes[i].turnaround_time = -1;    
        processes[i].waiting_time = -1;       
        processes[i].response_time = -1;
        processes[i].predicted_burst = -1;
        processes[i].has_started = 0;

        for (int j = 0; j < MAX_RUNTIME; j++) {
            processes[i].running_time[j] = 0;
        } 

        i++;
    }

    return i;

}

/*---------- FUNCTION: init_options ------------------------
/  Function Description:
/    Initializes each of the members of the options struct 
/    as an empty string. 
/  
/  Caller Input:
/    Options *options: pointer to options struct 
/  
/  Caller Output:
/    N/A - No output values
/  
/  Assumptions, Limitations, Known Bugs:
/    N/A
/---------------------------------------------------------*/
void init_options(Options *options) {
    strcpy(options->alg_selection, "");
    strcpy(options->input_file, "");
    strcpy(options->output_file, "");
}

/*---------- FUNCTION: parse_cli_args ----------------------
/  Function Description:
/    Parses command-line arguments and populates an options struct 
/    with the selected info. 
/  
/  Caller Input:
/    int argc: number of command-line args
/    char **argv: array of command-line args
/    Options *options: pointer to parsed CLI input 
/  
/  Caller Output:
/    0 on success, 1 on failure 
/  
/  Assumptions, Limitations, Known Bugs:
/    - validates algorithm names by transforming to uppercase 
/    - incorrect values will provide error messages and you will
/      have to rerun the program to correct them 
/    - prints to stderr to ensure it prints to the terminal 
/      when stdin is redirected 
/---------------------------------------------------------*/
int parse_cli_args(int argc, char **argv, Options *options) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-a") == 0) {
            if (i + 1 < argc) {
                strcpy(options->alg_selection, argv[++i]);
                str_to_upper(options->alg_selection);
            } else {
                fprintf(stderr, "Error: -a option requires an argument.");
                return 1;
            }
        } else if (strcmp(argv[i], "-i") == 0) {
            if (i + 1 < argc) {
                strcpy(options->input_file, argv[++i]);
            } else {
                fprintf(stderr, "Error: -i option requires an argument.\n");
                return 1;
            }
        } else if (strcmp(argv[i], "-o") == 0) {
            if (i + 1 < argc) {
                strcpy(options->output_file, argv[++i]);
            } else {
                fprintf(stderr, "Error: -o option requires an argument.\n");
                return 1;
            }
        } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_help();
        } else {
            fprintf(stderr, "Error: unknown selection. %s\n", argv[i]);
            return 1;
        }
    }
    return 0;
}

/*---------- FUNCTION: print_help --------------------------
/  Function Description:
/    Prints the details of the program for the user, describing 
/    the basics of how to use the program and the arguments the 
/    user can pass with option flags. 
/  
/  Caller Input:
/    N/A - No input parameters
/  
/  Caller Output:
/    N/A - No output values
/  
/  Assumptions, Limitations, Known Bugs:
/    - prints to stderr to ensure it prints to the terminal 
/      when stdin is redirected 
/---------------------------------------------------------*/
void print_help() {
    fprintf(stderr, "NAME\n\tsim - simulate process scheduling algorithms\n\n");
    fprintf(stderr, "SYNOPSIS\n\t./sim [-a algorithm_type] [-i file_name] [-o file_name] [-h | --help]\n\n");
    fprintf(stderr, "DESCRIPTION\n");
    fprintf(stderr, "\tSimulates various process scheduling algorithms. The user may "
           "provide input data\n\tthrough stdin or by redirecting data from an input file. "
           "Results may be provided\n\tto stdout or by redirecting data to an output file.\n\n");
    fprintf(stderr, "OPTIONS\n");
    fprintf(stderr, "\tThe following options are available:\n\n");
    fprintf(stderr, "\t-a algorithm_type\n");
    fprintf(stderr, "\t\tSpecify the algorithm type:\n\n");
    fprintf(stderr, "\t\tFCFS\t\tFirst Come First Serve\n");
    fprintf(stderr, "\t\tSJF\t\tShortest Job First\n");
    fprintf(stderr, "\t\tSRTF\t\tShortest Remaining Time First\n");
    fprintf(stderr, "\t\tRR\t\tRound Robin\n");
    fprintf(stderr, "\t\tPS\tPriority Scheduling\n");
    fprintf(stderr, "\t\tPPS\t\tPreemptive Priority Scheduling\n\n");
    fprintf(stderr, "\t-i file_name\n\t\tSpecify the full path of an input file.\n\n");
    fprintf(stderr, "\t-o file_name\n\t\tSpecify the full path of an output file.\n\n");
    fprintf(stderr, "\t-h, --help\n\t\tPrint detailed help info about the program.\n\n");
}

/*---------- FUNCTION: is_unique_id ------------------------
/  Function Description:
/    Checks whether a given process ID is unique amongst the 
/    other array entries. 
/  
/  Caller Input:
/    Process processes[]: array of process details 
/    int count: number of processes in the array 
/    char *id: the pid that needs to be validated as unique 
/  
/  Caller Output:
/    0 on success, 1 on failure 
/  
/  Assumptions, Limitations, Known Bugs:
/    - assumes the array contains valid data 
/---------------------------------------------------------*/
int is_unique_id(Process processes[], int count, char *id) {
    for (int i = 0; i < count; i++) {
        if (strcmp(processes[i].id, id) == 0) {
            return 0;
        }
    }
    return 1;
}

/*---------- FUNCTION: str_to_upper ------------------------
/  Function Description:
/    Converts the characters in a given string to uppercase.
/  
/  Caller Input:
/    char *str: the string to be made uppercase
/  
/  Caller Output:
/    N/A - No output values
/  
/  Assumptions, Limitations, Known Bugs:
/    N/A - simple, tested, works as intended 
/---------------------------------------------------------*/
void str_to_upper(char *str) {
    while (*str != '\0') {
        *str = toupper(*str);
        str++;
    }
}