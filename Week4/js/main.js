
document.addEventListener("DOMContentLoaded", function () {
    const sim = window.DeadlockSim;

    if (!sim) {
        alert("Resource management module did not load.");
        return;
    }

    const createButton = document.getElementById("createSystemBtn");
    const calculateButton = document.getElementById("calculateNeedBtn");
    const bankersButton = document.getElementById("bankersBtn");
    const detectButton = document.getElementById("detectBtn");
    const graphButton = document.getElementById("showGraphBtn");
    const recoverButton = document.getElementById("recoverBtn");
    const requestButton = document.getElementById("requestResourcesBtn");
    const statusButton = document.getElementById("statusBtn");

    // RESOURCE UTILIZATION ANALYSIS
    function updateResourceUtilization() {
        const state = sim.state;

        const summary = document.getElementById("utilizationSummary");
        const chart = document.getElementById("utilizationChart");
        const overallElement = document.getElementById("overallUtilization");
        const waitingElement = document.getElementById("waitingProcessCount");
        const deadlockedElement = document.getElementById("deadlockedProcessCount");

        if (!state || !summary || !chart) {
            return;
        }

        const total = Array.isArray(state.total) ? state.total : [];
        const available = Array.isArray(state.available) ? state.available : [];
        const allocation = Array.isArray(state.allocation) ? state.allocation : [];
        const requests = Array.isArray(state.request) ? state.request : [];

        const resourceCount = Number(state.resourceCount) || total.length;
        const processCount = Number(state.processCount) || allocation.length;

        let totalAllocated = 0;
        let totalResources = 0;
        let waitingProcesses = 0;

        let chartHTML = "";

        for (let j = 0; j < resourceCount; j++) {
            let allocated = 0;

            for (let i = 0; i < allocation.length; i++) {
                allocated += Number(
                    Array.isArray(allocation[i]) ? allocation[i][j] : 0
                ) || 0;
            }

            const resourceTotal = Number(total[j]) || 0;
            const resourceAvailable = Number(available[j]) || 0;

            let utilization = 0;

            if (resourceTotal > 0) {
                utilization = (allocated / resourceTotal) * 100;
            }

            // Keep the displayed bar within 0% to 100%.
            const barWidth = Math.max(0, Math.min(100, utilization));

            totalAllocated += allocated;
            totalResources += resourceTotal;

            let barClass = "utilization-fill";

            if (utilization >= 80) {
                barClass += " high";
            }

            if (utilization >= 100) {
                barClass += " full";
            }

            chartHTML += `
                <div class="utilization-resource">
                    <h3>Resource R${j}</h3>

                    <div class="utilization-details">
                        <div>
                            <span>Allocated</span>
                            <strong>${allocated}</strong>
                        </div>

                        <div>
                            <span>Available</span>
                            <strong>${resourceAvailable}</strong>
                        </div>

                        <div>
                            <span>Total</span>
                            <strong>${resourceTotal}</strong>
                        </div>
                    </div>

                    <div class="utilization-bar-header">
                        <span>Resource utilization</span>
                        <span>${utilization.toFixed(1)}%</span>
                    </div>

                    <div
                        class="utilization-track"
                        role="progressbar"
                        aria-label="Resource R${j} utilization"
                        aria-valuemin="0"
                        aria-valuemax="100"
                        aria-valuenow="${barWidth.toFixed(1)}"
                    >
                        <div
                            class="${barClass}"
                            style="width: ${barWidth}%"
                        ></div>
                    </div>
                </div>
            `;
        }

        // A process is waiting if any resource request is greater than zero.
        for (let i = 0; i < processCount; i++) {
            const processRequest = requests[i];

            if (Array.isArray(processRequest)) {
                let isWaiting = false;

                for (let j = 0; j < processRequest.length; j++) {
                    if ((Number(processRequest[j]) || 0) > 0) {
                        isWaiting = true;
                        break;
                    }
                }

                if (isWaiting) {
                    waitingProcesses++;
                }
            }
        }

        const overallUtilization = totalResources > 0
            ? (totalAllocated / totalResources) * 100
            : 0;

        const deadlockedProcesses = Array.isArray(state.deadlockedProcesses)
            ? state.deadlockedProcesses.length
            : 0;

        summary.innerHTML = `
            <div class="utilization-card">
                <span>Overall Utilization</span>
                <strong>${overallUtilization.toFixed(1)}%</strong>
            </div>

            <div class="utilization-card">
                <span>Waiting Processes</span>
                <strong>${waitingProcesses}</strong>
            </div>

            <div class="utilization-card">
                <span>Deadlocked Processes</span>
                <strong>${deadlockedProcesses}</strong>
            </div>
        `;

        chart.innerHTML = resourceCount > 0
            ? chartHTML
            : `<div class="utilization-empty">
                   Create a system to view resource utilization.
               </div>`;

        if (overallElement) {
            overallElement.textContent = overallUtilization.toFixed(1) + "%";
        }

        if (waitingElement) {
            waitingElement.textContent = waitingProcesses;
        }

        if (deadlockedElement) {
            deadlockedElement.textContent = deadlockedProcesses;
        }
    }

    // CREATE SYSTEM
    if (createButton) {
        createButton.addEventListener("click", function () {
            sim.createSystem();
            updateResourceUtilization();
        });
    }

    // CALCULATE NEED AND AVAILABLE
    if (calculateButton) {
        calculateButton.addEventListener("click", function () {
            sim.calculateNeedAvailable();
            updateResourceUtilization();
        });
    }

    // BANKER'S ALGORITHM
    if (bankersButton) {
        bankersButton.addEventListener("click", function () {
            if (typeof sim.runBankersAlgorithm === "function") {
                sim.runBankersAlgorithm();
                updateResourceUtilization();
            } else {
                alert("Banker's Algorithm module did not load.");
            }
        });
    }

    // DEADLOCK DETECTION
    if (detectButton) {
        detectButton.addEventListener("click", function () {
            if (typeof sim.detectDeadlock === "function") {
                sim.detectDeadlock();
                updateResourceUtilization();
            } else {
                alert("Deadlock detection module did not load.");
            }
        });
    }

    // RESOURCE ALLOCATION GRAPH
    if (graphButton) {
        graphButton.addEventListener("click", function () {
            if (typeof sim.showResourceAllocationGraph === "function") {
                sim.showResourceAllocationGraph();
                updateResourceUtilization();
            } else {
                alert("Resource Allocation Graph module did not load.");
            }
        });
    }

    // DEADLOCK RECOVERY
    if (recoverButton) {
        recoverButton.addEventListener("click", function () {
            if (typeof sim.recoverFromDeadlock === "function") {
                sim.recoverFromDeadlock();
                updateResourceUtilization();
            } else {
                alert("Deadlock recovery module did not load.");
            }
        });
    }

    // REQUEST RESOURCES
    if (requestButton) {
        requestButton.addEventListener("click", function () {
            if (typeof sim.requestResources === "function") {
                sim.requestResources();
                updateResourceUtilization();
            } else {
                alert("Resource request module did not load.");
            }
        });
    }

    // DISPLAY SYSTEM STATUS
    if (statusButton) {
        statusButton.addEventListener("click", function () {
            const state = sim.state;
            const result = document.getElementById("analysisResult");

            updateResourceUtilization();

            if (result && state) {
                const summary = document.getElementById("utilizationSummary");

                result.className = "result-box";

                result.textContent =
                    "System Status: " + (state.status || "Not calculated") +
                    " | Processes: " + (state.processCount || 0) +
                    " | Resource Types: " + (state.resourceCount || 0) +
                    " | " + (summary ? summary.innerText.replace(/\s+/g, " ").trim() : "");

                if (typeof sim.log === "function") {
                    sim.log("System status and resource utilization displayed.");
                }
            }
        });
    }

    // INITIAL DISPLAY
    updateResourceUtilization();

    if (typeof sim.log === "function") {
        sim.log("Deadlock Simulation System initialized.");
    }
});
