#include <stddef.h>
#include "deadlock.h"

/* Deadlock Recovery */
int recover_process(SystemState *state, int process_id)
{
    int j;

    /* Validate system state */
    if (state == NULL)
    {
        return 0;
    }

    if (state->process_count < 1 ||
        state->process_count > MAX_PROCESSES ||
        state->resource_count < 1 ||
        state->resource_count > MAX_RESOURCES)
    {
        return 0;
    }

    /* Check whether process ID is valid */
    if (process_id < 0 || process_id >= state->process_count)
    {
        return 0;
    }

    /* Check whether process is active */
    if (!state->active[process_id])
    {
        return 0;
    }

    /* Release allocated resources */
    for (j = 0; j < state->resource_count; j++)
    {
        state->available[j] +=
            state->allocation[process_id][j];

        state->allocation[process_id][j] = 0;
        state->request[process_id][j] = 0;
    }

    /* Mark process as inactive */
    state->active[process_id] = 0;

    /* Update Need Matrix */
    calculate_need(state);

    return 1;
}
