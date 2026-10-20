window.DeadlockSim = window.DeadlockSim || {};

(function (sim) {
    function isSafe(available, allocation, need) {
        const work = available.slice();
        const finish = Array(need.length).fill(false);
        let changed = true;

        while (changed) {
            changed = false;
            for (let i = 0; i < need.length; i++) {
                if (finish[i]) continue;
                if (need[i].every((v, j) => v <= work[j])) {
                    allocation[i].forEach((v, j) => work[j] += v);
                    finish[i] = true;
                    changed = true;
                }
            }
        }
        return finish.every(Boolean);
    }

    function refresh(state) {
        for (const [id, matrix, editable, prefix] of [
            ["allocationMatrix", state.allocation, true, "allocation"],
            ["maximumMatrix", state.maximum, true, "maximum"],
            ["needMatrix", state.need, false, "need"]
        ]) {
            const box = document.getElementById(id);
            if (!box) continue;

            let html = '<table class="matrix-table"><thead><tr><th>Process</th>';
            for (let j = 0; j < state.resourceCount; j++) {
                html += '<th>R' + j + '</th>';
            }
            html += '</tr></thead><tbody>';

            for (let i = 0; i < state.processCount; i++) {
                html += '<tr><th>P' + i + '</th>';
                for (let j = 0; j < state.resourceCount; j++) {
                    html += '<td>';
                    html += editable
                        ? '<input type="number" min="0" step="1" value="' +
                          matrix[i][j] + '" data-matrix="' + prefix +
                          '" data-row="' + i + '" data-col="' + j + '">'
                        : matrix[i][j];
                    html += '</td>';
                }
                html += '</tr>';
            }
            box.innerHTML = html + '</tbody></table>';
        }

        const box = document.getElementById("availableVector");
        if (box) {
            box.innerHTML = state.available.map((v, j) =>
                '<span class="vector-item">R' + j + ': ' + v + '</span>'
            ).join("");
        }
    }

    sim.recoverFromDeadlock = function () {
        const state = sim.state;

        if (!state || state.processCount === 0) {
            alert("Create a system first.");
            return;
        }

        const deadlocked = state.deadlockedProcesses || [];
        if (deadlocked.length === 0) {
            const result = document.getElementById("analysisResult");
            if (result) {
                result.className = "result-box safe";
                result.textContent =
                    "No deadlocked processes to recover. Run deadlock detection first.";
            }
            return;
        }

        let victim = -1;
        let largest = -1;

        for (const name of deadlocked) {
            const i = Number(String(name).replace("P", ""));
            if (!Number.isInteger(i) || i < 0 || i >= state.processCount) {
                continue;
            }
            const held = state.allocation[i].reduce((sum, v) => sum + v, 0);
            if (held > largest) {
                largest = held;
                victim = i;
            }
        }

        if (victim < 0) {
            alert("Could not identify a valid deadlocked process.");
            return;
        }

        for (let j = 0; j < state.resourceCount; j++) {
            state.available[j] += state.allocation[victim][j];
            state.allocation[victim][j] = 0;
            state.maximum[victim][j] = 0;
            state.need[victim][j] = 0;
            state.request[victim][j] = 0;
        }

        let granted = 0;
        for (let i = 0; i < state.processCount; i++) {
            if (i === victim) continue;
            const request = state.request[i].slice();

            if (!request.some(v => v > 0)) continue;
            if (!request.every((v, j) => v <= state.need[i][j] &&
                                          v <= state.available[j])) continue;

            const available = state.available.slice();
            const allocation = state.allocation.map(row => row.slice());
            const need = state.need.map(row => row.slice());

            request.forEach((v, j) => {
                available[j] -= v;
                allocation[i][j] += v;
                need[i][j] -= v;
            });

            if (isSafe(available, allocation, need)) {
                state.available = available;
                state.allocation = allocation;
                state.need = need;
                state.request[i] = Array(state.resourceCount).fill(0);
                granted++;
            }
        }

        state.safeSequence = [];
        refresh(state);

        const sequence = document.getElementById("safeSequenceSummary");
        if (sequence) sequence.textContent = "—";

        if (typeof sim.detectDeadlock === "function") {
            sim.detectDeadlock();
        }

        const remains = state.deadlockedProcesses.length > 0;
        state.status = remains ? "DEADLOCKED" : "RECOVERED";

        const status = document.getElementById("systemStatus");
        if (status) status.textContent = state.status;

        const result = document.getElementById("analysisResult");
        if (result) {
            result.className = remains
                ? "result-box deadlocked" : "result-box safe";
            result.textContent =
                "Recovery completed for P" + victim +
                ". Its resources were released. Pending requests granted: " +
                granted + "." +
                (remains
                    ? " Deadlock remains in: " +
                      state.deadlockedProcesses.join(", ") + "."
                    : " No deadlocked processes remain.");
        }

        sim.log("Recovery completed for P" + victim +
            ". Pending requests granted: " + granted + ".");
    };
})(window.DeadlockSim);
