#include <stddef.h>
#include "resource_manager.h"

/* Request Resources */
int request_resources(SystemState *state,
                      int process_id,
                      const int request[])
{
    int j;
    int safe_sequence[MAX_PROCESSES];
    int sequence_length;

    if (state == NULL || request == NULL)
        return -1;

    if (process_id < 0 ||
        process_id >= state->process_count ||
        !state->active[process_id])
        return -1;

    calculate_need(state);

    /* Check request against Need and Available */
    for (j = 0; j < state->resource_count; j++)
    {
        if (request[j] < 0 ||
            request[j] > state->need[process_id][j])
            return -1;

        if (request[j] > state->available[j])
            return 0;
    }

    /* Temporarily allocate resources */
    for (j = 0; j < state->resource_count; j++)
    {
        state->available[j] -= request[j];
        state->allocation[process_id][j] += request[j];
        state->need[process_id][j] -= request[j];
    }

    /* Check system safety */
    if (!banker_is_safe(state, safe_sequence, &sequence_length))
    {
        /* Undo allocation if system becomes unsafe */
        for (j = 0; j < state->resource_count; j++)
        {
            state->available[j] += request[j];
            state->allocation[process_id][j] -= request[j];
            state->need[process_id][j] += request[j];
        }

        return 0;
    }

    return 1;
}

/* Release Resources */
void release_resources(SystemState *state,
                       int process_id,
                       const int release[])
{
    int j;

    if (state == NULL || release == NULL)
        return;

    if (process_id < 0 ||
        process_id >= state->process_count ||
        !state->active[process_id])
        return;

    for (j = 0; j < state->resource_count; j++)
    {
        if (release[j] < 0 ||
            release[j] > state->allocation[process_id][j])
            return;
    }

    /* Return resources to Available */
    for (j = 0; j < state->resource_count; j++)
    {
        state->allocation[process_id][j] -= release[j];
        state->available[j] += release[j];
    }

    calculate_need(state);
}
