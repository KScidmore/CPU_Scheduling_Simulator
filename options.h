/*---------- ID HEADER -------------------------------------
/  Author(s):   Andrew Boisvert, Kyle Scidmore
/  Email(s):    abois526@mtroyal.ca, kscid125@mtroyal.ca 
/  File Name:   options.h
/
/  Program Purpose(s):
/    This header file describes the public interface of the accompanying
/    ".c" file, describing how this module may be used by others. 
/---------------------------------------------------------*/

#ifndef OPTIONS_H
#define OPTIONS_H

/*---------- STRUCTURE: Options ----------------------------
/  Structure Description:
/    This struct represents the command-line option flags that 
/    the program supports. The main function's argv is parsed 
/    and the struct either stores default values, or the user-provided 
/    values, depending on the result. 
/    
/  Fields:
/    char alg_selection[256]
/      The selection for the algorithm the user would like to run.
/      Options are: FCFS, SJF, SRTF, RR, PRIORITY, PP.
/      Input is not case-sensitive, as strings are converted to uppercase.
/    char input_file[256]
/      The full pathway of the input file.
/    char output_file[256]
/      The full pathway of the output file.
/  
/  Relationships:
/    N/A - this struct does not feature, and is not featured in, others 
/---------------------------------------------------------*/
typedef struct {
    char alg_selection[256];
    char input_file[256];
    char output_file[256];
} Options;


#endif