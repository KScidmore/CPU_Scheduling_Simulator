/*---------- ID HEADER -------------------------------------
/  Author(s):   Andrew Boisvert, Kyle Scidmore
/  Email(s):    abois526@mtroyal.ca, kscid125@mtroyal.ca 
/  File Name:   sim.h
/
/  Program Purpose(s):
/    This header file describes the public interface of the accompanying
/    ".c" file, describing how this module may be used by others. 
/---------------------------------------------------------*/

#ifndef SIM_H
#define SIM_H

#include "process.h"

int process_input(Process processes[], int choice);
int is_unique_id(Process processes[], int count, char *id);

#endif