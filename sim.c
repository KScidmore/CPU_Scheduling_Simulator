#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "sim.h"
#include "process.h"
#include "scheduling.h"
#include "globals.h"
#include "options.h"


int parse_cli_args(int argc, char **argv, Options *options);
void init_options(Options *options);
void print_help();
int process_input(Process processes[], int choice);
int process_file_input(Process processes[], Options *options);
void run_cli_mode(int argc, char **argv, Process processes[], Options *options);
void run_interactive_mode(Process processes[], Options *options);
void run_selected_algorithm(Process processes[], int num_processes, Options *options, FILE *fp);
void print_scheduling_menu();


int main(int argc, char **argv) {
    Process processes[MAX_PROCESSES];
    Options options;

    init_options(&options);

    if (argc > 1) {
        run_cli_mode(argc, argv, processes, &options);
    } else {
        run_interactive_mode(processes, &options);
    }

    return 0;
}


void run_cli_mode(int argc, char **argv, Process processes[], Options *options) {
    int num_processes = 0;
    int choice = 0;

    FILE *input = stdin;
    FILE *output = stdout;

    parse_cli_args(argc, argv, options);

    if (options->input_file[0] != '\0') {
        input = fopen(options->input_file, "r");
        num_processes = process_file_input(processes, options);
    } else if (!isatty(fileno(stdin))) {
        num_processes = process_file_input(processes, options);
    } else {
        // get interactively if null 
        if (options->alg_selection[0] == '\0') {
            print_scheduling_menu();
            scanf("%d", &choice);
            switch (choice) {
                case 1: strcpy(options->alg_selection, "fcfs"); break;
                case 2: strcpy(options->alg_selection, "sjf"); break;
                case 3: strcpy(options->alg_selection, "rr"); break;
                case 4: strcpy(options->alg_selection, "priority"); break;
                default: break;
            } 
        } 
        num_processes = process_input(processes, choice);
    }

    if (options->output_file[0] != '\0') {
        output = fopen(options->output_file, "w");
    }

    run_selected_algorithm(processes, num_processes, options, output);
}


void run_interactive_mode(Process processes[], Options *options) {
    int choice;
    int num_processes;

    FILE *output = stdout;

    print_scheduling_menu();
    scanf("%d", &choice);

    switch (choice) {
        case 1: strcpy(options->alg_selection, "fcfs"); break;
        case 2: strcpy(options->alg_selection, "sjf"); break;
        case 3: strcpy(options->alg_selection, "srtf"); break;
        case 4: strcpy(options->alg_selection, "rr"); break;
        case 5: strcpy(options->alg_selection, "priority"); break;
        case 6: strcpy(options->alg_selection, "pp"); break;
        default: break;
    }

    num_processes = process_input(processes, choice);

    run_selected_algorithm(processes, num_processes, options, output);
}

void print_scheduling_menu() {
    fprintf(stderr, "\nSelect a Scheduling Algorithm:\n");
    fprintf(stderr, "1. FCFS (First Come First Serve)\n");
    fprintf(stderr, "2. SJF (Shortest Job First)\n");
    fprintf(stderr, "3. SRTF (Shortest Remaining Time First)\n");
    fprintf(stderr, "4. RR (Round Robin)\n");
    fprintf(stderr, "5. Priority Scheduling\n");
    fprintf(stderr, "6. PP (Preemptive Priority)\n");
    fprintf(stderr, "\nEnter your choice (1-4): ");

}

void run_selected_algorithm(Process processes[], int num_processes, Options *options, FILE *fp) {
    if (strcmp(options->alg_selection, "fcfs") == 0) {
        simulate_FCFS(processes, num_processes, options, fp);
    } else if (strcmp(options->alg_selection, "sjf") == 0) {
        simulate_SJF(processes, num_processes, options, fp);
    } else if (strcmp(options->alg_selection, "rr") == 0) {
        simulate_round_robin(processes, num_processes, 2, fp);
    } else if (strcmp(options->alg_selection, "priority") == 0) {
        simulate_priority(processes, num_processes, options, fp);
    } else if (strcmp(options->alg_selection, "srtf") == 0){
        simulate_SRTF(processes, num_processes, fp);
    } else if (strcmp(options->alg_selection, "pp") == 0) {
        simulate_preemptive_priority(processes, num_processes, fp);
    } else{
         printf("Invalid choice.\n");
    }
}


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

    return num;
}

void init_options(Options *options) {
    strcpy(options->alg_selection, "");
    strcpy(options->input_file, "");
    strcpy(options->output_file, "");
}

int parse_cli_args(int argc, char **argv, Options *options) {
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            if (strcmp(argv[i], "-a") == 0) {
                if (i + 1 < argc) {
                    strcpy(options->alg_selection, argv[++i]);
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
    }
    return 0;
}


void print_help() {
    printf("NAME\n\tsim - simulate process scheduling algorithms\n\n");
    printf("SYNOPSIS\n\t./sim [-i filename] [-o filename] [-h | --help]\n\n");
    printf("DESCRIPTION\n");
    printf("\tSimulates various process scheduling algorithms. The user may "
           "provide input data \n\tthrough stdin or as an input file. "
           "Results may be provided to stdout or as an \n\toutput file.\n\n");
    printf("OPTIONS\n");
    printf("\tThe following options are available:\n\n");
    printf("\t-a arg\n\t\tSpecify the algorithm to test (sjf, fcfs, etc.\n\n");
    printf("\t-i filename\n\t\tSpecify the full path of an input file.\n\n");
    printf("\t-o filename\n\t\tSpecify the full path of an output file.\n\n");
    printf("\t-h, --help\n\t\tPrint detailed help info about the program.\n\n");
}


int process_file_input(Process processes[], Options *options) {

    FILE *input = stdin;
    if (options->input_file[0] != '\0') {
        input = fopen(options->input_file, "r");
    }

    int i = 0;

    //populates line and checks if it exists 
    while (fscanf(input, "%s %d %d", processes[i].id, &processes[i].arrival_time, &     processes[i].burst_time) == 3 && i <= MAX_PROCESSES) {
        processes[i].priority = -1;           
        processes[i].remaining_time = -1;
        processes[i].start_time = -1;         
        processes[i].completion_time = -1;    
        processes[i].turnaround_time = -1;    
        processes[i].waiting_time = -1;       
        processes[i].response_time = -1;
        processes[i].predicted_burst = -1;
        processes[i].has_started = 0;
        i++;
    }

    if (input != stdin) {
        fclose(input);
    }
    return i;
}

int is_unique_id(Process processes[], int count, char *id) {
    for (int i = 0; i < count; i++) {
        if (strcmp(processes[i].id, id) == 0) {
            return 0;
        }
    }
    return 1;
}