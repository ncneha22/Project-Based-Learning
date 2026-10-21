# Benchmark Results

## 1. Objective

To compare the previously recorded performance of the standalone
simulator with the multi-process simulator using IPC.

## 2. Previously Recorded Results

The following values were present in the earlier benchmark
document. They have not been independently reproduced during
the current integration testing.

| Measurement | Standalone | Multi-Process |
|---|---:|---:|
| Real Time | 0.016 s | 0.006 s |

## 3. Previously Recorded CPU and Memory Usage

### Standalone Simulator

- User time: 0.01 s
- System time: 0.00 s
- CPU usage: 100%
- Maximum memory usage: 1528 KB

### Multi-Process Simulator

- Core CPU usage: 0.0%
- UI CPU usage: 0.0%
- Logger CPU usage: 0.0%
- Core memory (RSS): 1948 KB
- UI memory (RSS): 1748 KB
- Logger memory (RSS): 1628 KB
- Total memory (RSS): 5316 KB (approximately 5.2 MB)

These are historical measurements from the earlier document,
not fresh measurements from the current test session.

## 4. IPC Overhead

The multi-process simulator uses a UNIX domain socket for
UI-to-Core commands, a POSIX message queue for Core-to-UI
responses, and a named FIFO for Core-to-Logger messages.

Inter-process communication and managing separate processes
can introduce additional overhead.

## 5. Performance Analysis

The standalone simulator runs within one process, while the
multi-process simulator separates UI, Core, and Logger.

The multi-process design provides process separation and
modular communication. However, the recorded timing values
alone are insufficient to establish a general performance
advantage.

A fair comparison requires the same workload, comparable
execution conditions, repeated measurements, and a documented
measurement method.

## 6. Conclusion

The historical benchmark values are retained for reference.
They should not be treated as freshly verified results.
The current integration tests verified the simulator's
functional operations and IPC communication, including
recording a division-by-zero error in the error log.
