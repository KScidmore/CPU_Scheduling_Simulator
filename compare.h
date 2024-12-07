/*---------- ID HEADER -------------------------------------
/  Author(s):   Andrew Boisvert, Kyle Scidmore
/  Email(s):    abois526@mtroyal.ca, kscid125@mtroyal.ca 
/  File Name:   compare.h
/
/  File Description:
/    This header file declares the comparison functions used for 
/    process scheduling.
/---------------------------------------------------------*/
#ifndef COMPARE_H
#define COMPARE_H

/*Prototypes*/
int compare_arrival(const void *a, const void *b);
int compare_burst(const void *a, const void *b);
int compare_completion(const void *a, const void *b);
int compare_priority(const void *a, const void *b);


#endif