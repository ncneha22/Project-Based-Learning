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

    if (createButton) {
        createButton.addEventListener("click", function () {
            sim.createSystem();
        });
    }

    if (calculateButton) {
        calculateButton.addEventListener("click", function () {
            sim.calculateNeedAvailable();
        });
    }

    if (bankersButton) {
        bankersButton.addEventListener("click", function () {
            if (typeof sim.runBankersAlgorithm === "function") {
                sim.runBankersAlgorithm();
            } else {
                alert("Banker's Algorithm module did not load.");
            }
        });
    }

    if (detectButton) {
        detectButton.addEventListener("click", function () {
            if (typeof sim.detectDeadlock === "function") {
                sim.detectDeadlock();
            } else {
                alert("Deadlock detection module did not load.");
            }
        });
    }

    if (graphButton) {
        graphButton.addEventListener("click", function () {
            if (typeof sim.showResourceAllocationGraph === "function") {
                sim.showResourceAllocationGraph();
            } else {
                alert("Resource Allocation Graph module did not load.");
            }
        });
    }

    if (recoverButton) {
        recoverButton.addEventListener("click", function () {
            if (typeof sim.recoverFromDeadlock === "function") {
                sim.recoverFromDeadlock();
            } else {
                alert("Deadlock recovery module did not load.");
            }
        });
    }

    if (requestButton) {
        requestButton.addEventListener("click", function () {
            if (typeof sim.requestResources === "function") {
                sim.requestResources();
            } else {
                alert("Resource request module did not load.");
            }
        });
    }

    if (statusButton) {
        statusButton.addEventListener("click", function () {
            const state = sim.state;
            const result = document.getElementById("analysisResult");

            if (result && state) {
                result.className = "result-box";
                result.textContent =
                    "System Status: " + state.status +
                    " | Processes: " + state.processCount +
                    " | Resource Types: " + state.resourceCount;

                sim.log("System status displayed: " + state.status);
            }
        });
    }

    sim.log("Deadlock Simulation System initialized.");
});
