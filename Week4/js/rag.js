
window.DeadlockSim = window.DeadlockSim || {};

(function (sim) {
    const SVG_NS = "http://www.w3.org/2000/svg";

    sim.showResourceAllocationGraph = function () {
        const state = sim.state;
        const container = document.getElementById("ragContainer");

        if (!state || state.processCount === 0) {
            alert("Create a system first.");
            return;
        }

        // Add styles without modifying style.css.
        if (!document.getElementById("ragSvgStyles")) {
            const style = document.createElement("style");
            style.id = "ragSvgStyles";
            style.textContent = `
                #ragContainer { overflow-x: auto; }
                .rag-svg { display: block; max-width: 100%; height: auto; }
                .rag-legend {
                    display: flex;
                    flex-wrap: wrap;
                    gap: 12px;
                    margin: 10px 0;
                    font-size: 13px;
                }
                .rag-legend span {
                    display: inline-flex;
                    align-items: center;
                    gap: 5px;
                }
                .rag-dot {
                    width: 11px;
                    height: 11px;
                    display: inline-block;
                    border-radius: 50%;
                }
                .rag-line {
                    width: 22px;
                    display: inline-block;
                    border-top: 3px solid;
                }
                .rag-line.dashed {
                    border-top-style: dashed;
                }
            `;
            document.head.appendChild(style);
        }

        const processCount = state.processCount;
        const resourceCount = state.resourceCount;

        // Calculate graph size and node positions.
        const width = Math.max(
            600,
            processCount * 100 + 60,
            resourceCount * 130 + 60
        );

        const height = 330;
        const processY = 75;
        const resourceY = 255;
        const processRadius = 27;
        const resourceWidth = 72;
        const resourceHeight = 48;

        const processX = [];
        const resourceX = [];

        for (let i = 0; i < processCount; i++) {
            processX.push((width * (i + 1)) / (processCount + 1));
        }

        for (let j = 0; j < resourceCount; j++) {
            resourceX.push((width * (j + 1)) / (resourceCount + 1));
        }

        // Create an SVG element.
        function svgElement(tag, attributes, parent) {
            const element = document.createElementNS(SVG_NS, tag);

            for (const key in attributes) {
                element.setAttribute(key, attributes[key]);
            }

            if (parent) {
                parent.appendChild(element);
            }

            return element;
        }

        // Add text to the graph.
        function addText(parent, x, y, value, attributes = {}) {
            const text = svgElement("text", {
                x: x,
                y: y,
                "text-anchor": "middle",
                "font-size": "14",
                fill: "#222",
                ...attributes
            }, parent);

            text.textContent = value;
            return text;
        }

        const svg = svgElement("svg", {
            viewBox: `0 0 ${width} ${height}`,
            width: width,
            height: height,
            role: "img",
            "aria-label": "Resource Allocation Graph",
            class: "rag-svg"
        });

        // Create arrowheads for allocation and request edges.
        const defs = svgElement("defs", {}, svg);

        function addMarker(id, colour) {
            const marker = svgElement("marker", {
                id: id,
                markerWidth: 8,
                markerHeight: 8,
                refX: 7,
                refY: 4,
                orient: "auto",
                markerUnits: "strokeWidth"
            }, defs);

            svgElement("path", {
                d: "M0,0 L8,4 L0,8 Z",
                fill: colour
            }, marker);
        }

        addMarker("ragAssignmentArrow", "#1976d2");
        addMarker("ragRequestArrow", "#e58b20");
        addMarker("ragDeadlockArrow", "#d32f2f");

        const edges = svgElement("g", {}, svg);
        const deadlocked = state.deadlockedProcesses || [];

        // Draw allocation arrows: Resource Rj -> Process Pi.
        for (let i = 0; i < processCount; i++) {
            for (let j = 0; j < resourceCount; j++) {
                const count = state.allocation[i][j];

                if (count <= 0) {
                    continue;
                }

                const isRed = deadlocked.includes("P" + i);
                const colour = isRed ? "#d32f2f" : "#1976d2";

                const dx = processX[i] - resourceX[j];
                const dy = processY - resourceY;
                const length = Math.sqrt(dx * dx + dy * dy);
                const ux = dx / length;
                const uy = dy / length;

                const startX = resourceX[j] + ux * (resourceHeight / 2);
                const startY = resourceY + uy * (resourceHeight / 2);
                const endX = processX[i] - ux * (processRadius + 4);
                const endY = processY - uy * (processRadius + 4);

                svgElement("line", {
                    x1: startX,
                    y1: startY,
                    x2: endX,
                    y2: endY,
                    stroke: colour,
                    "stroke-width": 2.5,
                    "marker-end": isRed
                        ? "url(#ragDeadlockArrow)"
                        : "url(#ragAssignmentArrow)"
                }, edges);

                addText(
                    edges,
                    (startX + endX) / 2 - 8,
                    (startY + endY) / 2 - 4,
                    String(count),
                    { fill: colour, "font-weight": "bold" }
                );
            }
        }

        // Draw request arrows: Process Pi -> Resource Rj.
        for (let i = 0; i < processCount; i++) {
            for (let j = 0; j < resourceCount; j++) {
                const count = state.request[i][j];

                if (count <= 0) {
                    continue;
                }

                // Highlight requests to resources held by deadlocked processes.
                let cycleEdge = false;

                if (deadlocked.includes("P" + i)) {
                    for (let k = 0; k < processCount; k++) {
                        if (
                            deadlocked.includes("P" + k) &&
                            state.allocation[k][j] > 0
                        ) {
                            cycleEdge = true;
                            break;
                        }
                    }
                }

                const colour = cycleEdge ? "#d32f2f" : "#e58b20";

                const dx = resourceX[j] - processX[i];
                const dy = resourceY - processY;
                const length = Math.sqrt(dx * dx + dy * dy);
                const ux = dx / length;
                const uy = dy / length;

                const startX = processX[i] + ux * (processRadius + 4);
                const startY = processY + uy * (processRadius + 4);
                const endX = resourceX[j] - ux * (resourceWidth / 2 + 4);
                const endY = resourceY - uy * (resourceHeight / 2 + 4);

                svgElement("line", {
                    x1: startX,
                    y1: startY,
                    x2: endX,
                    y2: endY,
                    stroke: colour,
                    "stroke-width": 2.5,
                    "stroke-dasharray": "7 5",
                    "marker-end": cycleEdge
                        ? "url(#ragDeadlockArrow)"
                        : "url(#ragRequestArrow)"
                }, edges);

                addText(
                    edges,
                    (startX + endX) / 2 + 8,
                    (startY + endY) / 2 - 4,
                    String(count),
                    { fill: colour, "font-weight": "bold" }
                );
            }
        }

        // Draw process circles.
        const nodes = svgElement("g", {}, svg);

        for (let i = 0; i < processCount; i++) {
            const isDeadlocked = deadlocked.includes("P" + i);

            svgElement("circle", {
                cx: processX[i],
                cy: processY,
                r: processRadius,
                fill: isDeadlocked ? "#ffdddd" : "#e3f2fd",
                stroke: isDeadlocked ? "#d32f2f" : "#1976d2",
                "stroke-width": isDeadlocked ? 3 : 2
            }, nodes);

            addText(nodes, processX[i], processY + 5, "P" + i, {
                "font-weight": "bold",
                fill: isDeadlocked ? "#b71c1c" : "#154c79"
            });
        }

        // Draw resource rectangles and their total instance counts.
        for (let j = 0; j < resourceCount; j++) {
            svgElement("rect", {
                x: resourceX[j] - resourceWidth / 2,
                y: resourceY - resourceHeight / 2,
                width: resourceWidth,
                height: resourceHeight,
                rx: 4,
                fill: "#fff3e0",
                stroke: "#e58b20",
                "stroke-width": 2
            }, nodes);

            addText(nodes, resourceX[j], resourceY - 3, "R" + j, {
                "font-weight": "bold"
            });

            addText(
                nodes,
                resourceX[j],
                resourceY + 15,
                "Total: " + state.total[j],
                { "font-size": "11" }
            );
        }

        // Display the legend and the graph.
        container.innerHTML = "";

        const legend = document.createElement("div");
        legend.className = "rag-legend";
        legend.innerHTML = `
            <span>
                <i class="rag-line" style="border-color:#1976d2"></i>
                Allocation
            </span>
            <span>
                <i class="rag-line dashed" style="border-color:#e58b20"></i>
                Request
            </span>
            <span>
                <i class="rag-dot" style="background:#d32f2f"></i>
                Deadlock
            </span>
        `;

        container.appendChild(legend);
        container.appendChild(svg);

        // Let deadlock.js know that the graph has been drawn.
        container.dataset.graphDrawn = "true";

        sim.log("Resource Allocation Graph displayed.");
    };
})(window.DeadlockSim);
