window.DeadlockSim = window.DeadlockSim || {};

(function (sim) {
    sim.runBankersAlgorithm = function () {
        const state = sim.state;

        if (state.processCount === 0) {
            alert("Create a system first.");
            return;
        }

        const work = state.available.slice();
        const finish = [];
        const safeSequence = [];

        for (let i = 0; i < state.processCount; i++) {
            finish.push(false);
        }

        let progress = true;

        while (progress) {
            progress = false;

            for (let i = 0; i < state.processCount; i++) {
                if (finish[i] === false) {
                    let canFinish = true;

                    for (let j = 0; j < state.resourceCount; j++) {
                        if (state.need[i][j] > work[j]) {
                            canFinish = false;
                            break;
                        }
                    }

                    if (canFinish) {
                        for (let j = 0; j < state.resourceCount; j++) {
                            work[j] += state.allocation[i][j];
                        }

                        finish[i] = true;
                        safeSequence.push("P" + i);
                        progress = true;
                    }
                }
            }
        }

        let allFinished = true;

        for (let i = 0; i < state.processCount; i++) {
            if (finish[i] === false) {
                allFinished = false;
                break;
            }
        }

        const result = document.getElementById("analysisResult");
        const status = document.getElementById("systemStatus");
        const sequence = document.getElementById("safeSequenceSummary");

        if (allFinished) {
            state.status = "SAFE";
            state.safeSequence = safeSequence;

            status.textContent = "SAFE";
            sequence.textContent = safeSequence.join(" → ");

            result.className = "result-box safe";
            result.textContent =
                "SAFE STATE. Safe sequence: " +
                safeSequence.join(" → ");

            sim.log("Banker's Algorithm: system is SAFE.");
        } else {
            state.status = "UNSAFE";
            state.safeSequence = safeSequence;

            status.textContent = "UNSAFE";
            sequence.textContent = "No complete safe sequence";

            result.className = "result-box unsafe";
            result.textContent =
                "UNSAFE STATE. Not all processes can finish.";

            sim.log("Banker's Algorithm: system is UNSAFE.");
        }
    };
})(window.DeadlockSim);