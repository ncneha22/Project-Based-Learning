/*
 * bankers.js - Banker's Algorithm module (Member 2)
 *
 * This file contains:
 *   1. calculateNeed     - builds the Need matrix (Max - Allocation)
 *   2. checkSafety       - Banker's safety algorithm (SAFE / UNSAFE + safe sequence)
 *   3. requestResources  - checks a resource request and grants it only if the
 *                          system stays SAFE
 *   4. runBankersAlgorithm - small GUI wrapper used by the "Run Banker's
 *                          Algorithm" button in main.js
 *
 * Matrix format used everywhere:
 *   available  = [R0, R1, ...]                 (one number per resource type)
 *   allocation = [[P0R0, P0R1, ...], [P1R0, ...], ...]  (one row per process)
 *   maximum    = same shape as allocation
 *   need       = same shape as allocation
 *
 * The algorithm functions never change the arrays they are given.
 * They work on copies and return a plain result object instead.
 */

window.DeadlockSim = window.DeadlockSim || {};

(function (sim) {
    const Bankers = {};

    /* ---------- Small helpers ---------- */

    // True if value is a whole number that is 0 or more.
    function isNonNegativeInteger(value) {
        return Number.isInteger(value) && value >= 0;
    }

    // Copy a 1D array.
    function copyVector(vector) {
        return vector.slice();
    }

    // Copy a 2D array (each row is copied too).
    function copyMatrix(matrix) {
        return matrix.map(function (row) {
            return row.slice();
        });
    }

    // Check a vector: must be an array of the given length
    // containing only non-negative integers.
    function checkVector(vector, length, name) {
        if (!Array.isArray(vector)) {
            return name + " must be an array.";
        }

        if (vector.length !== length) {
            return name + " must have " + length + " values, but has " +
                vector.length + ".";
        }

        for (let j = 0; j < vector.length; j++) {
            if (!isNonNegativeInteger(vector[j])) {
                return name + "[" + j + "] must be a non-negative integer " +
                    "(found " + vector[j] + ").";
            }
        }

        return null;
    }

    // Check a matrix: must have the given number of rows and columns
    // and contain only non-negative integers.
    function checkMatrix(matrix, rows, columns, name) {
        if (!Array.isArray(matrix) || matrix.length !== rows) {
            return name + " must have " + rows + " rows.";
        }

        for (let i = 0; i < rows; i++) {
            const error = checkVector(matrix[i], columns, name + " row P" + i);

            if (error !== null) {
                return error;
            }
        }

        return null;
    }

    /*
     * validateState(available, allocation, maximum)
     * Returns null if the data is valid, otherwise an error message.
     */
    Bankers.validateState = function (available, allocation, maximum) {
        if (!Array.isArray(allocation) || allocation.length === 0) {
            return "Allocation matrix must have at least one process.";
        }

        if (!Array.isArray(allocation[0]) || allocation[0].length === 0) {
            return "Allocation matrix must have at least one resource type.";
        }

        const processCount = allocation.length;
        const resourceCount = allocation[0].length;

        let error = checkVector(available, resourceCount, "Available");
        if (error !== null) return error;

        error = checkMatrix(allocation, processCount, resourceCount, "Allocation");
        if (error !== null) return error;

        error = checkMatrix(maximum, processCount, resourceCount, "Maximum");
        if (error !== null) return error;

        for (let i = 0; i < processCount; i++) {
            for (let j = 0; j < resourceCount; j++) {
                if (allocation[i][j] > maximum[i][j]) {
                    return "Allocation is greater than Maximum for P" + i +
                        ", R" + j + ".";
                }
            }
        }

        return null;
    };

    /*
     * calculateNeed(maximum, allocation)
     * Need[i][j] = Max[i][j] - Allocation[i][j]
     * Returns { ok: true, need } or { ok: false, error }.
     */
    Bankers.calculateNeed = function (maximum, allocation) {
        if (!Array.isArray(allocation) || allocation.length === 0 ||
            !Array.isArray(allocation[0])) {
            return { ok: false, error: "Allocation matrix is empty or invalid." };
        }

        const processCount = allocation.length;
        const resourceCount = allocation[0].length;

        let error = checkMatrix(allocation, processCount, resourceCount, "Allocation");
        if (error === null) {
            error = checkMatrix(maximum, processCount, resourceCount, "Maximum");
        }
        if (error !== null) {
            return { ok: false, error: error };
        }

        const need = [];

        for (let i = 0; i < processCount; i++) {
            need.push([]);

            for (let j = 0; j < resourceCount; j++) {
                const value = maximum[i][j] - allocation[i][j];

                if (value < 0) {
                    return {
                        ok: false,
                        error: "Allocation is greater than Maximum for P" + i +
                            ", R" + j + "."
                    };
                }

                need[i].push(value);
            }
        }

        return { ok: true, need: need };
    };

    /*
     * checkSafety(available, allocation, maximum)
     * Banker's safety algorithm.
     *
     * Returns:
     * {
     *   ok: true,
     *   status: "SAFE" or "UNSAFE",
     *   safeSequence: ["P1", "P3", ...],   (complete only when SAFE)
     *   need: [[...]],
     *   explanation: "text"
     * }
     * or { ok: false, status: "INVALID", error: "text" } for bad data.
     */
    Bankers.checkSafety = function (available, allocation, maximum) {
        const error = Bankers.validateState(available, allocation, maximum);

        if (error !== null) {
            return { ok: false, status: "INVALID", error: error };
        }

        const need = Bankers.calculateNeed(maximum, allocation).need;
        const processCount = allocation.length;
        const resourceCount = available.length;

        // Step 1: Work = copy of Available (the real Available is not touched).
        const work = copyVector(available);

        // Step 2: every process starts as unfinished.
        const finish = [];
        for (let i = 0; i < processCount; i++) {
            finish.push(false);
        }

        const safeSequence = [];
        let foundOne = true;

        // Step 6: keep going while we can still finish some process.
        while (foundOne) {
            foundOne = false;

            for (let i = 0; i < processCount; i++) {
                if (finish[i]) {
                    continue;
                }

                // Step 3: can this process get everything it still needs?
                let canFinish = true;

                for (let j = 0; j < resourceCount; j++) {
                    if (need[i][j] > work[j]) {
                        canFinish = false;
                        break;
                    }
                }

                if (canFinish) {
                    // Step 4: pretend it finishes and gives back its resources.
                    for (let j = 0; j < resourceCount; j++) {
                        work[j] += allocation[i][j];
                    }

                    finish[i] = true;

                    // Step 5: record it in the safe sequence.
                    safeSequence.push("P" + i);
                    foundOne = true;
                }
            }
        }

        // Steps 7 and 8: SAFE only if every process could finish.
        const unfinished = [];
        for (let i = 0; i < processCount; i++) {
            if (!finish[i]) {
                unfinished.push("P" + i);
            }
        }

        if (unfinished.length === 0) {
            return {
                ok: true,
                status: "SAFE",
                safeSequence: safeSequence,
                need: need,
                explanation: "All processes can finish. Safe sequence: " +
                    safeSequence.join(" → ")
            };
        }

        return {
            ok: true,
            status: "UNSAFE",
            safeSequence: safeSequence,
            need: need,
            explanation: "No complete safe sequence exists. " +
                unfinished.join(", ") +
                " cannot be guaranteed to finish."
        };
    };

    /*
     * requestResources(state, processIndex, request)
     *
     * state   = { available, allocation, maximum }
     * request = [R0, R1, ...] requested by process processIndex
     *
     * The original state is never changed. If the request is granted,
     * the new values are returned in result.newState and the caller
     * (main.js / resourceManager.js) decides whether to save them.
     *
     * Returns:
     * {
     *   granted: true/false,
     *   status: "SAFE", "UNSAFE", "WAIT" (not enough available) or "INVALID",
     *   reason: "text",
     *   safeSequence: [...],
     *   newState: { available, allocation, maximum, need } or null
     * }
     */
    Bankers.requestResources = function (state, processIndex, request) {
        function reject(status, reason) {
            return {
                granted: false,
                status: status,
                reason: reason,
                safeSequence: [],
                newState: null
            };
        }

        if (!state) {
            return reject("INVALID", "No system state was given.");
        }

        const error = Bankers.validateState(
            state.available, state.allocation, state.maximum);

        if (error !== null) {
            return reject("INVALID", error);
        }

        const processCount = state.allocation.length;
        const resourceCount = state.available.length;

        if (!Number.isInteger(processIndex) ||
            processIndex < 0 || processIndex >= processCount) {
            return reject("INVALID", "Invalid process index: " + processIndex + ".");
        }

        const requestError = checkVector(request, resourceCount, "Request");
        if (requestError !== null) {
            return reject("INVALID", requestError);
        }

        const need = Bankers.calculateNeed(state.maximum, state.allocation).need;

        // Check 1: Request <= Need
        for (let j = 0; j < resourceCount; j++) {
            if (request[j] > need[processIndex][j]) {
                return reject("INVALID",
                    "P" + processIndex + " requested " + request[j] + " of R" + j +
                    " but its remaining Need is only " + need[processIndex][j] + ".");
            }
        }

        // Check 2: Request <= Available
        for (let j = 0; j < resourceCount; j++) {
            if (request[j] > state.available[j]) {
                return reject("WAIT",
                    "Not enough R" + j + " available (requested " + request[j] +
                    ", available " + state.available[j] + "). P" + processIndex +
                    " must wait.");
            }
        }

        // Check 3: pretend to allocate, using copies only.
        const newAvailable = copyVector(state.available);
        const newAllocation = copyMatrix(state.allocation);

        for (let j = 0; j < resourceCount; j++) {
            newAvailable[j] -= request[j];
            newAllocation[processIndex][j] += request[j];
        }

        // Check 4: recalculate Need and run the safety algorithm.
        const safety = Bankers.checkSafety(newAvailable, newAllocation, state.maximum);

        // Check 5 / 6: grant only if SAFE. The original state was never changed,
        // so rejecting needs no rollback.
        if (safety.status !== "SAFE") {
            return reject("UNSAFE",
                "Granting this request would leave the system UNSAFE, so P" +
                processIndex + " must wait.");
        }

        return {
            granted: true,
            status: "SAFE",
            reason: "Request granted. The system stays SAFE. Safe sequence: " +
                safety.safeSequence.join(" → "),
            safeSequence: safety.safeSequence,
            newState: {
                available: newAvailable,
                allocation: newAllocation,
                maximum: copyMatrix(state.maximum),
                need: safety.need
            }
        };
    };

    sim.Bankers = Bankers;

    /*
     * runBankersAlgorithm()
     * GUI wrapper called by main.js when the user clicks
     * "Run Banker's Algorithm". It reads sim.state, calls checkSafety,
     * and shows the result on the page.
     */
    sim.runBankersAlgorithm = function () {
        const state = sim.state;

        if (!state || state.processCount === 0) {
            alert("Create a system first.");
            return;
        }

        const result = Bankers.checkSafety(
            state.available, state.allocation, state.maximum);

        const resultBox = document.getElementById("analysisResult");
        const statusText = document.getElementById("systemStatus");
        const sequenceText = document.getElementById("safeSequenceSummary");

        if (!result.ok) {
            resultBox.className = "result-box unsafe";
            resultBox.textContent = "Invalid data: " + result.error;
            sim.log("Banker's Algorithm: invalid data. " + result.error);
            return;
        }

        state.status = result.status;
        state.safeSequence = result.safeSequence;
        statusText.textContent = result.status;

        if (result.status === "SAFE") {
            sequenceText.textContent = result.safeSequence.join(" → ");
            resultBox.className = "result-box safe";
            resultBox.textContent = "SAFE STATE. " + result.explanation;
            sim.log("Banker's Algorithm: system is SAFE.");
        } else {
            sequenceText.textContent = "No complete safe sequence";
            resultBox.className = "result-box unsafe";
            resultBox.textContent = "UNSAFE STATE. " + result.explanation;
            sim.log("Banker's Algorithm: system is UNSAFE.");
        }
    };
})(window.DeadlockSim);
