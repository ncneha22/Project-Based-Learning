
window.DeadlockSim = window.DeadlockSim || {};

(function (sim) {
    sim.detectDeadlock = function () {
        const state = sim.state;

        if (!state || state.processCount === 0) {
            alert("Create a system first.");
            return;
        }

        const work = state.available.slice();
        const finish = [];

        // Initially mark processes with no allocated resources as finished.
        for (let i = 0; i < state.processCount; i++) {
            let hasAllocation = false;

            for (let j = 0; j < state.resourceCount; j++) {
                if (state.allocation[i][j] > 0) {
                    hasAllocation = true;
                    break;
                }
            }

            finish.push(!hasAllocation);
        }

        // Check whether waiting processes can finish with available resources.
        let progress = true;

        while (progress) {
            progress = false;

            for (let i = 0; i < state.processCount; i++) {
                if (finish[i] === false) {
                    let canFinish = true;

                    for (let j = 0; j < state.resourceCount; j++) {
                        if (state.request[i][j] > work[j]) {
                            canFinish = false;
                            break;
                        }
                    }

                    if (canFinish) {
                        // Release the resources held by this finished process.
                        for (let j = 0; j < state.resourceCount; j++) {
                            work[j] += state.allocation[i][j];
                        }

                        finish[i] = true;
                        progress = true;
                    }
                }
            }
        }

        // Collect the processes that could not finish.
        const deadlocked = [];

        for (let i = 0; i < state.processCount; i++) {
            if (finish[i] === false) {
                deadlocked.push("P" + i);
            }
        }

        state.deadlockedProcesses = deadlocked;

        const result = document.getElementById("analysisResult");
        const status = document.getElementById("systemStatus");

        if (deadlocked.length === 0) {
            result.className = "result-box safe";
            result.textContent =
                "DEADLOCK DETECTION: No deadlocked processes found.";

            // Clear the previous deadlock status if one was set.
            if (state.status === "DEADLOCKED") {
                state.status = "NOT RUN";
                status.textContent = "NOT RUN";
            }

            sim.log("Deadlock detection: no deadlocked processes found.");
        } else {
            state.status = "DEADLOCKED";
            status.textContent = "DEADLOCKED";

            // Build messages showing which deadlocked process holds
            // a resource requested by another deadlocked process.
            const waitingChain = [];

            for (let i = 0; i < state.processCount; i++) {
                if (!deadlocked.includes("P" + i)) {
                    continue;
                }

                for (let j = 0; j < state.resourceCount; j++) {
                    if (state.request[i][j] <= 0) {
                        continue;
                    }

                    for (let k = 0; k < state.processCount; k++) {
                        if (
                            k !== i &&
                            deadlocked.includes("P" + k) &&
                            state.allocation[k][j] > 0
                        ) {
                            waitingChain.push(
                                "P" + i + " waits for R" + j +
                                " held by P" + k
                            );
                        }
                    }
                }
            }

            result.className = "result-box deadlocked";
            result.textContent =
                "DEADLOCK DETECTED! Processes: " + deadlocked.join(", ");

            if (waitingChain.length > 0) {
                result.textContent +=
                    ". Waiting chain: " + waitingChain.join("; ");
            }

            sim.log("Deadlock detected in: " + deadlocked.join(", "));
        }

        // Refresh the graph if it has already been displayed.
        const graphContainer = document.getElementById("ragContainer");

        if (
            graphContainer &&
            graphContainer.dataset.graphDrawn === "true" &&
            typeof sim.showResourceAllocationGraph === "function"
        ) {
            sim.showResourceAllocationGraph();
        }
    };
})(window.DeadlockSim);
