# Inter-Process Communication (IPC)
## Project-Based Learning
This repository contains our Project-Based Learning on Inter-Process Communication (IPC).

## Team Members and Contributions

| Name | Register Number | Role | Contribution |
|---|---|---|---|
| **Neha NC** | **25191135** | **Team Lead** | Coordinated the overall project and integrated the UI, Core, and Logger components into a single working multi-process simulator. Worked on POSIX IPC integration, resolved communication and interface issues, performed system-level testing, maintained IPC architecture and test documentation, carried out standalone and multi-process benchmarking, analyzed performance results, and handled the final GitHub integration and updates. |
| **Sree Charan** | **25191126** | **UI Process** | Developed the User Interface process for interacting with the simulator. Implemented the menu-based interface for accepting user commands and inputs, sending operations to the Core process through IPC, and displaying responses received from the Core. Worked on UI-to-Core communication and ensured that the simulator operations could be accessed through the interface. |
| **Mohammed Safaf** | **25191131** | **Core Process** | Developed the Core process responsible for the main simulator operations. Implemented and integrated the CPU, Memory, Stack, and Queue components and handled commands received from the UI. Worked on the IPC communication for receiving UI commands, sending responses back to the UI, and forwarding execution information to the Logger process. Tested the core operations and ensured that the simulator logic worked correctly with the multi-process architecture. |
| **Mansi** | **25191129** | **Logging Process** | Developed the Logger process responsible for receiving and recording execution and error information from the Core process. Implemented the Core-to-Logger logging mechanism using the selected IPC method, displayed received log messages, and maintained log files for recording simulator activity. Ensured that messages generated during simulator operations were correctly received and recorded. |

