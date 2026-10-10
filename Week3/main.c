
#include <stdio.h>
#include "process.h"

int main() {
    int n;
    int i;
    int choice;
    int quantum;

    printf("PROCESS SCHEDULING SIMULATOR\n");
    printf("============================\n");

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    if (n <= 0) {
        printf("Invalid number of processes!\n");
        return 1;
    }

    Process p[n];

    for (i = 0; i < n; i++) {
        printf("\nEnter details for Process %d\n", i + 1);

        printf("Enter PID: ");
        scanf("%d", &p[i].pid);

        printf("Enter Arrival Time: ");
        scanf("%d", &p[i].arrival_time);

        printf("Enter Burst Time: ");
        scanf("%d", &p[i].burst_time);

        printf("Enter Priority: ");
        scanf("%d", &p[i].priority);

        if (p[i].arrival_time < 0 || p[i].burst_time <= 0) {
            printf("Invalid arrival time or burst time!\n");
            return 1;
        }
    }

    printf("\nSCHEDULING ALGORITHMS\n");
    printf("=====================\n");
    printf("1. FCFS\n");
    printf("2. SJF Non-Preemptive\n");
    printf("3. SJF Preemptive\n");
    printf("4. Priority Non-Preemptive\n");
    printf("5. Priority Preemptive\n");
    printf("6. Round Robin\n");

    printf("\nEnter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            fcfs(p, n);
            break;

        case 2:
            sjf_non_preemptive(p, n);
            break;

        case 3:
            sjf_preemptive(p, n);
            break;

        case 4:
            priority_non_preemptive(p, n);
            break;

        case 5:
            priority_preemptive(p, n);
            break;

        case 6:
            printf("Enter time quantum: ");
            scanf("%d", &quantum);

            if (quantum <= 0) {
                printf("Invalid time quantum!\n");
                return 1;
            }

            round_robin(p, n, quantum);
            break;

        default:
            printf("Invalid choice!\n");
            return 1;
    }

    return 0;
}








