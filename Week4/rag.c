#include <stddef.h>
#include "deadlock.h"

/* Build Resource Allocation Graph */
int build_rag_edges(SystemState *state,
                    RAGEdge edges[],
                    int capacity)
{
    int i, j;
    int count = 0;

    if (state == NULL || edges == NULL || capacity < 0)
    {
        return -1;
    }

    if (state->process_count < 1 ||
        state->process_count > MAX_PROCESSES ||
        state->resource_count < 1 ||
        state->resource_count > MAX_RESOURCES)
    {
        return -1;
    }

    for (i = 0; i < state->process_count; i++)
    {
        if (!state->active[i])
            continue;

        for (j = 0; j < state->resource_count; j++)
        {
            /* Resource allocated to process */
            if (state->allocation[i][j] > 0)
            {
                if (count >= capacity)
                    return -1;

                edges[count].process_id = i;
                edges[count].resource_id = j;
                edges[count].edge_type = ALLOCATION_EDGE;

                count++;
            }

            /* Process requesting a resource */
            if (state->request[i][j] > 0)
            {
                if (count >= capacity)
                    return -1;

                edges[count].process_id = i;
                edges[count].resource_id = j;
                edges[count].edge_type = REQUEST_EDGE;

                count++;
            }
        }
    }

    return count;
}
