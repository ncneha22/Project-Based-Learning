
window.DeadlockSim = window.DeadlockSim || {};

(function (sim) {
    sim.state = {
        processCount: 0,
        resourceCount: 0,
        total: [],
        available: [],
        allocation: [],
        maximum: [],
        need: [],
        request: [],
        safeSequence: [],
        deadlockedProcesses: [],
        status: "NOT RUN",
        dataReady: false
    };

    // Invalidate calculated results when the user edits resource data.
    document.addEventListener("input", function (event) {
        const target = event.target;

        if (!target || typeof target.matches !== "function") return;

        if (!target.matches(
            '[data-matrix="allocation"], [data-matrix="maximum"], [data-total-resource]'
        )) return;

        const state = sim.state;
        if (state.processCount === 0) return;

        state.dataReady = false;
        state.status = "NOT RUN";
        state.safeSequence = [];
        state.deadlockedProcesses = [];
        state.request = createMatrix(
            state.processCount,
            state.resourceCount
        );

        const status = document.getElementById("systemStatus");
        const sequence = document.getElementById("safeSequenceSummary");
        const needMatrix = document.getElementById("needMatrix");
        const availableVector = document.getElementById("availableVector");
        const result = document.getElementById("analysisResult");

        if (status) status.textContent = "NOT RUN";
        if (sequence) sequence.textContent = "—";
        if (needMatrix) needMatrix.textContent =
            "Input changed. Recalculate Need and Available.";
        if (availableVector) availableVector.textContent =
            "Input changed. Recalculate Need and Available.";

        if (result) {
            result.className = "result-box";
            result.textContent =
                "Resource data changed. Click Calculate Need & Available before running an analysis or making a request.";
        }
    });

    sim.log = function (message) {
        const logBox = document.getElementById("systemLog");
        if (!logBox) return;

        const entry = document.createElement("p");
        entry.textContent = message;
        logBox.prepend(entry);
    };

    function createMatrix(rows, columns) {
        const matrix = [];

        for (let i = 0; i < rows; i++) {
            const row = [];

            for (let j = 0; j < columns; j++) {
                row.push(0);
            }

            matrix.push(row);
        }

        return matrix;
    }

    function makeMatrixTable(containerId, matrix, editable, prefix) {
        const container = document.getElementById(containerId);
        const state = sim.state;

        let html = '<table class="matrix-table"><thead><tr><th>Process</th>';

        for (let j = 0; j < state.resourceCount; j++) {
            html += "<th>R" + j + "</th>";
        }

        html += "</tr></thead><tbody>";

        for (let i = 0; i < state.processCount; i++) {
            html += "<tr><th>P" + i + "</th>";

            for (let j = 0; j < state.resourceCount; j++) {
                html += "<td>";

                if (editable) {
                    html += '<input type="number" min="0" step="1" value="' +
                        matrix[i][j] +
                        '" data-matrix="' + prefix +
                        '" data-row="' + i +
                        '" data-col="' + j +
                        '" aria-label="' + prefix + ' process ' + i +
                        ' resource ' + j + '">';
                } else {
                    html += matrix[i][j];
                }

                html += "</td>";
            }

            html += "</tr>";
        }

        html += "</tbody></table>";
        container.innerHTML = html;
    }

    function makeResourceInputs() {
        const container = document.getElementById("resourceInputs");
        let html = "";

        for (let j = 0; j < sim.state.resourceCount; j++) {
            html += '<label class="resource-field">Resource R' + j +
                '<input type="number" min="0" step="1" value="3" data-total-resource="' +
                j + '"></label>';
        }

        container.innerHTML = html;
    }

    function makeRequestInputs() {
        const container = document.getElementById("requestInputs");
        let html = "";

        for (let j = 0; j < sim.state.resourceCount; j++) {
            html += '<label class="resource-field">R' + j +
                '<input type="number" min="0" step="1" value="0" data-request-resource="' +
                j + '"></label>';
        }

        container.innerHTML = html;
    }

    function makeProcessOptions() {
        const select = document.getElementById("requestProcess");
        let html = "";

        for (let i = 0; i < sim.state.processCount; i++) {
            html += '<option value="' + i + '">Process P' + i + '</option>';
        }

        select.innerHTML = html;
    }

    function updateAvailableVector() {
        document.getElementById("availableVector").innerHTML =
            sim.state.available.map(function (value, index) {
                return '<span class="vector-item">R' + index + ": " +
                    value + "</span>";
            }).join("");
    }

    sim.createSystem = function () {
        const processCount = Number(document.getElementById("processCount").value);
        const resourceCount = Number(document.getElementById("resourceCount").value);

        if (!Number.isInteger(processCount) || processCount < 1 || processCount > 10 ||
            !Number.isInteger(resourceCount) || resourceCount < 1 || resourceCount > 5) {
            alert("Enter 1–10 processes and 1–5 resource types.");
            return false;
        }

        const state = sim.state;

        state.processCount = processCount;
        state.resourceCount = resourceCount;
        state.total = Array(resourceCount).fill(3);
        state.available = Array(resourceCount).fill(3);
        state.allocation = createMatrix(processCount, resourceCount);
        state.maximum = createMatrix(processCount, resourceCount);
        state.need = createMatrix(processCount, resourceCount);
        state.request = createMatrix(processCount, resourceCount);
        state.safeSequence = [];
        state.deadlockedProcesses = [];
        state.status = "NOT RUN";
        state.dataReady = false;

        makeResourceInputs();
        makeMatrixTable("allocationMatrix", state.allocation, true, "allocation");
        makeMatrixTable("maximumMatrix", state.maximum, true, "maximum");
        makeMatrixTable("needMatrix", state.need, false, "need");
        makeRequestInputs();
        makeProcessOptions();

        document.getElementById("processSummary").textContent = processCount;
        document.getElementById("resourceSummary").textContent = resourceCount;
        document.getElementById("safeSequenceSummary").textContent = "—";
        document.getElementById("systemStatus").textContent = "NOT RUN";
        document.getElementById("analysisResult").textContent =
            "System created. Enter Allocation and Maximum values.";
        document.getElementById("availableVector").textContent =
            "Calculate after entering the matrices.";

        sim.log("Created " + processCount + " processes and " +
            resourceCount + " resource types.");

        return true;
    };

    sim.calculateNeedAvailable = function () {
        const state = sim.state;

        if (state.processCount === 0) {
            alert("Create a system first.");
            return false;
        }

        const allocationInputs = document.querySelectorAll('[data-matrix="allocation"]');
        const maximumInputs = document.querySelectorAll('[data-matrix="maximum"]');
        const totalInputs = document.querySelectorAll("[data-total-resource]");

        function readInput(input) {
            const value = Number(input.value);

            if (input.value.trim() === "" ||
                !Number.isInteger(value) || value < 0) {
                return null;
            }

            return value;
        }

        const allocation = createMatrix(state.processCount, state.resourceCount);
        const maximum = createMatrix(state.processCount, state.resourceCount);
        const total = Array(state.resourceCount).fill(0);

        for (const input of allocationInputs) {
            const row = Number(input.dataset.row);
            const col = Number(input.dataset.col);
            const value = readInput(input);

            if (value === null) {
                alert("Allocation values must be non-negative integers.");
                return false;
            }

            allocation[row][col] = value;
        }

        for (const input of maximumInputs) {
            const row = Number(input.dataset.row);
            const col = Number(input.dataset.col);
            const value = readInput(input);

            if (value === null) {
                alert("Maximum values must be non-negative integers.");
                return false;
            }

            maximum[row][col] = value;
        }

        for (const input of totalInputs) {
            const col = Number(input.dataset.totalResource);
            const value = readInput(input);

            if (value === null) {
                alert("Total resource values must be non-negative integers.");
                return false;
            }

            total[col] = value;
        }

        const available = total.slice();

        for (let i = 0; i < state.processCount; i++) {
            for (let j = 0; j < state.resourceCount; j++) {
                if (maximum[i][j] < allocation[i][j]) {
                    alert("Maximum cannot be less than Allocation for P" +
                        i + ", R" + j + ".");
                    return false;
                }

                if (maximum[i][j] > total[j]) {
                    alert("Maximum for P" + i + ", R" + j +
                        " cannot exceed the system's total resource count.");
                    return false;
                }

                available[j] -= allocation[i][j];
            }
        }

        for (let j = 0; j < state.resourceCount; j++) {
            if (available[j] < 0) {
                alert("Allocated resources exceed the total for R" + j + ".");
                return false;
            }
        }

        const need = createMatrix(state.processCount, state.resourceCount);

        for (let i = 0; i < state.processCount; i++) {
            for (let j = 0; j < state.resourceCount; j++) {
                need[i][j] = maximum[i][j] - allocation[i][j];
            }
        }

        state.allocation = allocation;
        state.maximum = maximum;
        state.total = total;
        state.available = available;
        state.need = need;
        state.request = createMatrix(state.processCount, state.resourceCount);
        state.safeSequence = [];
        state.deadlockedProcesses = [];
        state.status = "NOT RUN";
        state.dataReady = true;

        makeMatrixTable("allocationMatrix", state.allocation, true, "allocation");
        makeMatrixTable("maximumMatrix", state.maximum, true, "maximum");
        makeMatrixTable("needMatrix", state.need, false, "need");
        updateAvailableVector();

        document.getElementById("systemStatus").textContent = "NOT RUN";
        document.getElementById("analysisResult").textContent =
            "Need Matrix and Available Resource Vector calculated.";

        sim.log("Allocation, Maximum, Need and Available values updated.");

        return true;
    };

    sim.requestResources = function () {
        const state = sim.state;

        if (state.processCount === 0) {
            alert("Create a system first.");
            return;
        }

        if (state.dataReady !== true) {
            alert("Enter valid resource data and click Calculate Need & Available first.");
            return;
        }

        const process = Number(document.getElementById("requestProcess").value);

        if (!Number.isInteger(process) ||
            process < 0 ||
            process >= state.processCount) {
            alert("Select a valid process for the resource request.");
            return;
        }

        const inputs = document.querySelectorAll("[data-request-resource]");
        const request = Array(state.resourceCount).fill(0);

        for (const input of inputs) {
            const resource = Number(input.dataset.requestResource);
            const value = Number(input.value);

            if (input.value.trim() === "" ||
                !Number.isInteger(value) || value < 0) {
                alert("Requests must be non-negative integers.");
                return;
            }

            request[resource] = value;
        }

        for (let j = 0; j < state.resourceCount; j++) {
            if (request[j] > state.need[process][j]) {
                alert("Request exceeds the remaining Need of P" + process + ".");
                return;
            }

            if (request[j] > state.available[j]) {
                state.request[process] = request.slice();
                state.status = "WAITING";

                const status = document.getElementById("systemStatus");
                const sequence = document.getElementById("safeSequenceSummary");
                const result = document.getElementById("analysisResult");

                if (status) status.textContent = "WAITING";
                if (sequence) sequence.textContent = "—";

                if (result) {
                    result.className = "result-box";
                    result.textContent =
                        "Request for P" + process +
                        " is waiting because the required resources are not currently available. Run deadlock detection to inspect pending requests.";
                }

                sim.log("Resource request for P" + process +
                    " is waiting for available resources.");
                return;
            }
        }

        const oldAvailable = state.available.slice();
        const oldAllocation = state.allocation[process].slice();
        const oldNeed = state.need[process].slice();

        for (let j = 0; j < state.resourceCount; j++) {
            state.available[j] -= request[j];
            state.allocation[process][j] += request[j];
            state.need[process][j] -= request[j];
        }

        const work = state.available.slice();
        const finish = Array(state.processCount).fill(false);
        const sequence = [];

        let progress = true;

        while (progress) {
            progress = false;

            for (let i = 0; i < state.processCount; i++) {
                if (!finish[i]) {
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
                        sequence.push("P" + i);
                        progress = true;
                    }
                }
            }
        }

        let safe = true;

        for (let i = 0; i < state.processCount; i++) {
            if (!finish[i]) {
                safe = false;
                break;
            }
        }

        const result = document.getElementById("analysisResult");

        if (safe) {
            state.request[process] = Array(state.resourceCount).fill(0);
            state.safeSequence = sequence;
            state.status = "SAFE";

            makeMatrixTable("allocationMatrix", state.allocation, true, "allocation");
            makeMatrixTable("needMatrix", state.need, false, "need");
            updateAvailableVector();

            document.getElementById("systemStatus").textContent = "SAFE";
            document.getElementById("safeSequenceSummary").textContent =
                sequence.join(" → ");

            result.className = "result-box safe";
            result.textContent = "Request granted to P" + process +
                ". The system remains in a safe state.";

            sim.log("Resource request granted to P" + process + ".");
        } else {
            state.available = oldAvailable;
            state.allocation[process] = oldAllocation;
            state.need[process] = oldNeed;
            state.request[process] = Array(state.resourceCount).fill(0);

            result.className = "result-box unsafe";
            result.textContent =
                "Request denied. It would make the system unsafe.";

            sim.log("Resource request denied to P" + process + ".");
        }
    };
})(window.DeadlockSim);
