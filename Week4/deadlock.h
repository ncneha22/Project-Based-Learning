#ifndef DEADLOCK_H
#define DEADLOCK_H

#define MAX_PROCESSES 10
#define MAX_RESOURCES 10

typedef struct
{
    int process_count;
    int resource_count;

    int total[MAX_RESOURCES];
    int available[MAX_RESOURCES];

    int allocation[MAX_PROCESSES][MAX_RESOURCES];
    int maximum[MAX_PROCESSES][MAX_RESOURCES];
    int need[MAX_PROCESSES][MAX_RESOURCES];
    int request[MAX_PROCESSES][MAX_RESOURCES];

    int active[MAX_PROCESSES];

} SystemState;

typedef struct
{
    int process_id;
    int resource_id;
    int edge_type;

} RAGEdge;

#define ALLOCATION_EDGE 0
#define REQUEST_EDGE 1

/* Banker's Algorithm */
void calculate_need(SystemState *state);

int banker_is_safe(SystemState *state,
                   int safe_sequence[],
                   int *sequence_length);

/* Deadlock Detection */
int detect_deadlock(SystemState *state, int deadlocked[]);

/* Resource Allocation Graph */
int build_rag_edges(SystemState *state,
                    RAGEdge edges[],
                    int capacity);

/* Recovery */
int recover_process(SystemState *state, int process_id);

#endif
