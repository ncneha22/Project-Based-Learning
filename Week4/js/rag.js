
window.DeadlockSim = window.DeadlockSim || {};

(function (sim) {
    sim.showResourceAllocationGraph = function () {
        const state = sim.state;
        const container = document.getElementById("ragContainer");

        if (state.processCount === 0) {
            alert("Create a system first.");
            return;
        }

        let html = '<div class="rag-legend">' +
            '<p><strong>Circle:</strong> Process</p>' +
            '<p><strong>Rectangle:</strong> Resource</p>' +
            '<p><strong>Resource → Process:</strong> Allocated resource</p>' +
            '<p><strong>Process → Resource:</strong> Resource request</p>' +
            '</div>';

        html += '<div class="matrix-container"><table class="matrix-table">' +
            '<thead><tr><th>Process</th>';

        for (let j = 0; j < state.resourceCount; j++) {
            html += '<th>R' + j + '</th>';
        }

        html += '</tr></thead><tbody>';

        for (let i = 0; i < state.processCount; i++) {
            html += '<tr><th>P' + i + '</th>';

            for (let j = 0; j < state.resourceCount; j++) {
                const allocated = state.allocation[i][j];
                const requested = state.request[i][j];

                html += '<td>';

                if (allocated > 0) {
                    html += 'Allocated: ' + allocated;
                }

                if (requested > 0) {
                    if (allocated > 0) html += '<br>';
                    html += 'Requested: ' + requested;
                }

                if (allocated === 0 && requested === 0) {
                    html += '—';
                }

                html += '</td>';
            }

            html += '</tr>';
        }

        html += '</tbody></table></div>';

        container.innerHTML = html;

        sim.log("Resource Allocation Graph displayed.");
    };
})(window.DeadlockSim);
