
#include <stdio.h>
#include "process.h"

void display_gantt_chart(ScheduleResult *result)
{
    int i;

    printf("\nGANTT CHART\n");
    printf("-----------\n");

    if (result == NULL || result->segment_count == 0) {
        printf("No execution segments recorded.\n");
        return;
    }

    for (i = 0; i < result->segment_count; i++) {
        if (result->segments[i].pid == -1)
            printf("| IDLE ");
        else
            printf("| P%d ", result->segments[i].pid);
    }

    printf("|\n");
    printf("%d", result->segments[0].start_time);

    for (i = 0; i < result->segment_count; i++)
        printf("\t%d", result->segments[i].end_time);

    printf("\n");
}

void display_results(Process p[], int n, ScheduleResult *result)
{
    int i;
    int total_burst = 0;
    int total_idle = 0;
    int total_busy = 0;
    int total_elapsed = 0;
    int first_start, last_end;

    float total_tat = 0;
    float total_wt = 0;
    float total_rt = 0;
    float utilization = 0;

    if (p == NULL || result == NULL || n <= 0)
        return;

    printf("\nPROCESS RESULTS\n");
    printf("----------------------------------------------------------------\n");
    printf("PID\tAT\tBT\tPR\tCT\tTAT\tWT\tRT\n");
    printf("----------------------------------------------------------------\n");

    for (i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid,
               p[i].arrival_time,
               p[i].burst_time,
               p[i].priority,
               p[i].completion_time,
               p[i].turnaround_time,
               p[i].waiting_time,
               p[i].response_time);

        total_tat += p[i].turnaround_time;
        total_wt += p[i].waiting_time;
        total_rt += p[i].response_time;
        total_burst += p[i].burst_time;
    }

    printf("----------------------------------------------------------------\n");
    printf("Average Turnaround Time: %.2f\n", total_tat / n);
    printf("Average Waiting Time: %.2f\n", total_wt / n);
    printf("Average Response Time: %.2f\n", total_rt / n);

    first_start = 0;
    last_end = result->segments[result->segment_count - 1].end_time;

    if (result->segment_count > 0) {
        total_elapsed = last_end - first_start;
        total_busy = total_burst;
        total_idle = total_elapsed - total_busy;

        if (total_idle < 0)
            total_idle = 0;

        if (total_elapsed > 0)
            utilization = ((float)total_busy / total_elapsed) * 100.0f;
    }

    printf("\nPERFORMANCE METRICS\n");
    printf("-------------------\n");
    printf("CPU Busy Time: %d\n", total_busy);
    printf("CPU Idle Time: %d\n", total_idle);
    printf("Total Elapsed Time: %d\n", total_elapsed);
    printf("CPU Utilization: %.2f%%\n", utilization);
}
