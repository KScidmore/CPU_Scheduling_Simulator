#ifndef DISPLAY_H
#define DISPLAY_H

void display_metrics(Process processes[], int num_processes, int idle_time, int current_time, FILE *fp);
void display_chart(Process processes[], int num_processes, FILE *fp);
void display_chart_file(Process processes[], int num_processes, FILE *fp);
void display_preemptive_chart(Process processes[], int num_processes, FILE *fp);

#endif