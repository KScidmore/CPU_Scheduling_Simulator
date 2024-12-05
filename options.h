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

typedef struct {
    char alg_selection[256];
    char input_file[256];
    char output_file[256];
} Options;


#endif