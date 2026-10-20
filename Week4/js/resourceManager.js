
/*
 * Week 4 - Resource Manager
 * Member 4
 *
 * Handles resource allocation and release.
 * Does not implement the Banker's algorithm.
 *
 * Expected shared state:
 * {
 *   total: [10, 5],
 *   available: [10, 5],
 *   processes: [
 *     { id: "P1", allocation: [0, 0], active: true }
 *   ]
 * }
 *
 * Resource IDs can be indexes (0, 1, ...) or names
 * such as R1, R2, ... . Names are one-based.
 */

function validateState(state) {
    if (!state || !Array.isArray(state.total) ||
        !Array.isArray(state.available) ||
        !Array.isArray(state.processes)) {
        throw new Error(
            "Invalid state: total, available, and processes are required."
        );
    }

    const count = state.total.length;

    if (count === 0 || state.available.length !== count) {
        throw new Error("Inconsistent resource dimensions.");
    }

    for (const vector of [state.total, state.available]) {
        for (const value of vector) {
            if (!Number.isInteger(value) || value < 0) {
                throw new Error(
                    "Resource quantities must be non-negative integers."
                );
            }
        }
    }

    for (const process of state.processes) {
        if (!Array.isArray(process.allocation) ||
            process.allocation.length !== count) {
            throw new Error(
                `Invalid allocation vector for process ${process.id}.`
            );
        }

        for (const value of process.allocation) {
            if (!Number.isInteger(value) || value < 0) {
                throw new Error(
                    `Invalid allocation quantity for process ${process.id}.`
                );
            }
        }
    }

    // Check the resource invariant:
    // Total = Available + allocations of active processes.
    for (let r = 0; r < count; r++) {
        let used = 0;

        for (const process of state.processes) {
            if (process.active !== false) {
                used += process.allocation[r];
            }
        }

        if (state.available[r] + used !== state.total[r]) {
            throw new Error(
                `Resource invariant violated for resource R${r + 1}.`
            );
        }
    }
}

function findProcess(state, processId) {
    const process = state.processes.find(
        p => String(p.id) === String(processId)
    );

    if (!process) {
        throw new Error(`Unknown process ID: ${processId}`);
    }

    if (process.active === false ||
        process.terminated === true) {
        throw new Error(`Process ${processId} is not active.`);
    }

    return process;
}

function findResourceIndex(state, resourceId) {
    let index;

    if (Number.isInteger(resourceId)) {
        index = resourceId;
    } else if (typeof resourceId === "string") {
        if (Array.isArray(state.resourceIds)) {
            index = state.resourceIds.findIndex(
                id => String(id) === resourceId
            );
        } else {
            const match = resourceId.match(/^R(\d+)$/i);
            if (match) {
                index = Number(match[1]) - 1;
            }
        }
    }

    if (!Number.isInteger(index) ||
        index < 0 ||
        index >= state.total.length) {
        throw new Error(`Unknown resource ID: ${resourceId}`);
    }

    return index;
}

function validateQuantity(quantity) {
    if (!Number.isInteger(quantity) || quantity <= 0) {
        throw new Error(
            "Quantity must be a positive integer."
        );
    }
}

/**
 * Allocate resources to a process.
 * All validation happens before the state is changed.
 */
export function allocateResource(
    state, processId, resourceId, quantity
) {
    validateState(state);
    validateQuantity(quantity);

    const process = findProcess(state, processId);
    const r = findResourceIndex(state, resourceId);

    if (state.available[r] < quantity) {
        throw new Error("Insufficient available resources.");
    }

    // Update only after all checks have passed.
    state.available[r] -= quantity;
    process.allocation[r] += quantity;

    return {
        success: true,
        message: `Allocated ${quantity} unit(s) of R${r + 1} to ${processId}.`,
        processId: process.id,
        resourceIndex: r,
        quantity,
        available: [...state.available],
        allocation: [...process.allocation]
    };
}

/**
 * Release resources held by a process.
 */
export function releaseResource(
    state, processId, resourceId, quantity
) {
    validateState(state);
    validateQuantity(quantity);

    const process = findProcess(state, processId);
    const r = findResourceIndex(state, resourceId);

    if (process.allocation[r] < quantity) {
        throw new Error(
            "Cannot release more resources than the process holds."
        );
    }

    // Update only after all checks have passed.
    process.allocation[r] -= quantity;
    state.available[r] += quantity;

    return {
        success: true,
        message: `Released ${quantity} unit(s) of R${r + 1} from ${processId}.`,
        processId: process.id,
        resourceIndex: r,
        quantity,
        available: [...state.available],
        allocation: [...process.allocation]
    };
}
