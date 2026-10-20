
/*
 * Week 4 - Deadlock Recovery
 * Member 4
 *
 * Recovers from a deadlock by terminating a selected process
 * and returning its allocated resources to Available.
 *
 * Expected shared state:
 * {
 *   total: [10, 5],
 *   available: [5, 3],
 *   processes: [
 *     {
 *       id: "P1",
 *       allocation: [5, 2],
 *       request: [1, 0],
 *       active: true
 *     }
 *   ],
 *   history: []
 * }
 */

function validateRecoveryState(state) {
    if (!state ||
        !Array.isArray(state.total) ||
        !Array.isArray(state.available) ||
        !Array.isArray(state.processes)) {
        throw new Error(
            "Invalid state: total, available, and processes are required."
        );
    }

    const resourceCount = state.total.length;

    if (resourceCount === 0 ||
        state.available.length !== resourceCount) {
        throw new Error("Inconsistent resource dimensions.");
    }

    for (const value of [...state.total, ...state.available]) {
        if (!Number.isInteger(value) || value < 0) {
            throw new Error(
                "Resource quantities must be non-negative integers."
            );
        }
    }

    for (const process of state.processes) {
        if (!Array.isArray(process.allocation) ||
            process.allocation.length !== resourceCount) {
            throw new Error(
                `Invalid allocation vector for process ${process.id}.`
            );
        }

        for (const value of process.allocation) {
            if (!Number.isInteger(value) || value < 0) {
                throw new Error(
                    `Invalid allocation for process ${process.id}.`
                );
            }
        }

        // Validate a request vector if the process has one.
        if (process.request !== undefined) {
            if (!Array.isArray(process.request) ||
                process.request.length !== resourceCount ||
                process.request.some(
                    value => !Number.isInteger(value) || value < 0
                )) {
                throw new Error(
                    `Invalid request vector for process ${process.id}.`
                );
            }
        }
    }

    // Check that resources have not already been over-allocated.
    for (let r = 0; r < resourceCount; r++) {
        let allocated = 0;

        for (const process of state.processes) {
            if (process.active !== false &&
                process.terminated !== true) {
                allocated += process.allocation[r];
            }
        }

        if (state.available[r] + allocated !== state.total[r]) {
            throw new Error(
                `Resource invariant violated for resource R${r + 1}.`
            );
        }
    }
}

/**
 * Terminate one process and release all resources it holds.
 *
 * @param {object} state - Shared simulator state.
 * @param {string|number} processId - ID of the process to terminate.
 * @returns {object} Structured recovery result.
 */
export function recoverByTermination(state, processId) {
    validateRecoveryState(state);

    const process = state.processes.find(
        p => String(p.id) === String(processId)
    );

    if (!process) {
        throw new Error(`Unknown process ID: ${processId}`);
    }

    if (process.active === false || process.terminated === true) {
        throw new Error(`Process ${processId} is already inactive.`);
    }

    // Prepare changes before modifying shared state.
    const released = [...process.allocation];
    const newAvailable = state.available.map(
        (available, r) => available + released[r]
    );

    const resourceCount = state.total.length;

    // Validate the proposed result before committing it.
    for (let r = 0; r < resourceCount; r++) {
        if (newAvailable[r] > state.total[r]) {
            throw new Error(
                `Recovery would exceed the total for resource R${r + 1}.`
            );
        }
    }

    // Commit the recovery.
    state.available.splice(0, resourceCount, ...newAvailable);
    process.allocation.fill(0);

    process.active = false;
    process.terminated = true;

    // Clear outstanding requests if the process stores them.
    if (Array.isArray(process.request)) {
        process.request.fill(0);
    }

    if (Array.isArray(process.need)) {
        process.need.fill(0);
    }

    // Preserve the process and record its recovery history.
    if (!Array.isArray(state.history)) {
        state.history = [];
    }

    const result = {
        success: true,
        action: "terminate",
        processId: process.id,
        releasedResources: released,
        available: [...state.available],
        message: `Process ${processId} terminated; its resources were released.`
    };

    state.history.push({
        ...result,
        releasedResources: [...released],
        available: [...state.available],
        timestamp: new Date().toISOString()
    });

    return result;
}
