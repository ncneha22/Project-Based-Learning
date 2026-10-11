
#include <stdio.h>
#include "process.h"

void display_gantt_chart(ScheduleResult *result) {
    int i;

    printf("\nGANTT CHART\n");
    printf("-----------\n");

    if (result->segment_count == 0) {
        printf("No execution segments recorded.\n");
        return;
    }

    for (i = 0; i < result->segment_count; i++) {
        if (result->segments[i].pid == -1) {
            printf("| IDLE ");
        } else {
            printf("| P%d ", result->segments[i].pid);
        }
    }
    printf("|\n");

    printf("%d", result->segments[0].start_time);

    for (i = 0; i < result->segment_count; i++) {
        printf("\t%d", result->segments[i].end_time);
    }
    printf("\n");
}

void display_results(Process p[], int n, ScheduleResult *result) {
    int i;
    float total_tat = 0;
    float total_wt = 0;
    float total_rt = 0;
    int total_burst = 0;

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

    if (result->segment_count > 0) {
        int first_start = result->segments[0].start_time;
        int last_end =
            result->segments[result->segment_count - 1].end_time;
        int elapsed = last_end - first_start;

        if (elapsed > 0) {
            float utilization =
                ((float)total_burst / elapsed) * 100.0f;

            printf("CPU Utilization: %.2f%%\n", utilization);
            printf("CPU Idle Time: %d\n", elapsed - total_burst);
        }
    }
}
