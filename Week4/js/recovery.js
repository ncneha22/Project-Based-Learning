
window.DeadlockSim = window.DeadlockSim || {};

(function (sim) {
    sim.recoverFromDeadlock = function () {
        const state = sim.state;

        if (state.processCount === 0) {
            alert("Create a system first.");
            return;
        }

        const deadlocked = state.deadlockedProcesses;

        if (deadlocked.length === 0) {
            const result = document.getElementById("analysisResult");
            result.className = "result-box safe";
            result.textContent =
                "No deadlocked processes to recover. Run deadlock detection first.";
            return;
        }

        const victim = Number(deadlocked[0].substring(1));

        for (let j = 0; j < state.resourceCount; j++) {
            state.available[j] += state.allocation[victim][j];
            state.allocation[victim][j] = 0;
            state.maximum[victim][j] = 0;
            state.need[victim][j] = 0;
            state.request[victim][j] = 0;
        }

        state.deadlockedProcesses.shift();
        state.status = "RECOVERED";

        document.getElementById("systemStatus").textContent = "RECOVERED";
        document.getElementById("safeSequenceSummary").textContent = "—";

        document.getElementById("availableVector").innerHTML =
            state.available.map(function (value, index) {
                return '<span class="vector-item">R' + index + ": " +
                    value + "</span>";
            }).join("");

        const result = document.getElementById("analysisResult");
        result.className = "result-box safe";
        result.textContent =
            "Recovery completed for process P" + victim +
            ". Its allocated resources have been released.";

        sim.log("Recovery completed for process P" + victim + ".");
    };
})(window.DeadlockSim);
