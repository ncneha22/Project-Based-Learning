
window.DeadlockSim = window.DeadlockSim || {};

(function (sim) {
    sim.detectDeadlock = function () {
        const state = sim.state;

        if (state.processCount === 0) {
            alert("Create a system first.");
            return;
        }

        const work = state.available.slice();
        const finish = [];

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
                        for (let j = 0; j < state.resourceCount; j++) {
                            work[j] += state.allocation[i][j];
                        }

                        finish[i] = true;
                        progress = true;
                    }
                }
            }
        }

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

            sim.log("Deadlock detection: no deadlocked processes found.");
        } else {
            state.status = "DEADLOCKED";
            status.textContent = "DEADLOCKED";

            result.className = "result-box deadlocked";
            result.textContent =
                "DEADLOCK DETECTED! Processes: " + deadlocked.join(", ");

            sim.log("Deadlock detected in: " + deadlocked.join(", "));
        }
    };
})(window.DeadlockSim);
