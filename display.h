/*---------- ID HEADER -------------------------------------
/  Author(s):   Andrew Boisvert, Kyle Scidmore
/  Email(s):    abois526@mtroyal.ca, kscid125@mtroyal.ca 
/  File Name:   display.h
/
/  File Description:
/    This header file declares the functions used for displaying
/    process scheduling metrics and charts.
/---------------------------------------------------------*/
#ifndef DISPLAY_H
#define DISPLAY_H

void display_metrics(Process processes[], int num_processes, int idle_time, int current_time, FILE *fp);
void display_chart(Process processes[], int num_processes, FILE *fp);
void display_chart_file(Process processes[], int num_processes, FILE *fp);
void display_preemptive_chart(Process processes[], int num_processes, FILE *fp);

#endif