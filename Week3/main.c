
#include <stdio.h>
#include "process.h"

int main() {
    int n, i, choice, quantum = 0;
    ScheduleResult result;

    printf("PROCESS SCHEDULING SIMULATOR\n");
    printf("============================\n");

    printf("Enter the number of processes: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Invalid number of processes!\n");
        return 1;
    }

    Process p[n];

    for (i = 0; i < n; i++) {
        printf("\nEnter details for Process %d\n", i + 1);

        printf("Enter PID: ");
        if (scanf("%d", &p[i].pid) != 1) return 1;

        printf("Enter Arrival Time: ");
        if (scanf("%d", &p[i].arrival_time) != 1) return 1;

        printf("Enter Burst Time: ");
        if (scanf("%d", &p[i].burst_time) != 1) return 1;

        printf("Enter Priority: ");
        if (scanf("%d", &p[i].priority) != 1) return 1;

        if (p[i].arrival_time < 0 || p[i].burst_time <= 0) {
            printf("Invalid arrival time or burst time!\n");
            return 1;
        }
    }

    printf("\nSCHEDULING ALGORITHMS\n");
    printf("=====================\n");
    printf("1. FCFS\n");
    printf("2. SJF Non-Preemptive\n");
    printf("3. SJF Preemptive (SRTF)\n");
    printf("4. Priority Non-Preemptive\n");
    printf("5. Priority Preemptive\n");
    printf("6. Round Robin\n");

    printf("\nEnter your choice: ");
    if (scanf("%d", &choice) != 1) return 1;

    result.segment_count = 0;

    switch (choice) {
        case 1:
            fcfs(p, n, &result);
            break;

        case 2:
            sjf_non_preemptive(p, n, &result);
            break;

        case 3:
            sjf_preemptive(p, n, &result);
            break;

        case 4:
            priority_non_preemptive(p, n, &result);
            break;

        case 5:
            priority_preemptive(p, n, &result);
            break;

        case 6:
            printf("Enter time quantum: ");
            if (scanf("%d", &quantum) != 1 || quantum <= 0) {
                printf("Invalid time quantum!\n");
                return 1;
            }

            round_robin(p, n, quantum, &result);
            break;

        default:
            printf("Invalid choice!\n");
            return 1;
    }

    printf("\nScheduling algorithm finished.\n");
    display_gantt_chart(&result);
    display_results(p, n, &result);

    return 0;
}
