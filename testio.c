#include <stdio.h>
#include <string.h>

typedef struct {
    char algorithm[256];
    char input_file[256];
    char output_file[256];
} Options;


void init_options(Options *options);
void set_options1(Options *options);

int main(int argc, char **argv) {

    Options options;
    init_options(&options);

    if (options.input_file[0] != '\0') {
        if (freopen(options.input_file, "r", stdin) == NULL) {
            fprintf(stderr, "Error: input file did not open.\n");
        }
    }

    if (options.output_file[0] != '\0') {
        if (freopen(options.output_file, "w", stdout) == NULL) {
            fprintf(stderr, "Error: output file did not open.\n");
        }
    }


    printf("%s\n%s\n%s\n", options.algorithm, options.input_file, options.output_file);

    return 0;
}

void init_options(Options *options) {
    strcpy(options->algorithm, "");
    strcpy(options->input_file, "");
    strcpy(options->output_file, "");
}

void set_options1(Options *options) {
    strcpy(options->algorithm, "fcfs");
    strcpy(options->input_file, "test-input.txt");
    strcpy(options->output_file, "test-output.txt");
}
