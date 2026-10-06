# Benchmark Results

## Objective

To compare the performance of the standalone simulator with the multi-process simulator using IPC.

## Benchmark Results

| Measurement | Standalone | Multi-Process |
|---|---:|---:|
| Real Time | 0.016 s | 0.006 s |

## CPU and Memory Usage

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

## IPC Overhead

The multi-process simulator introduces IPC communication between the UI, Core, and Logger processes. IPC and process management introduce additional system overhead.

The measured workload was small, so the timing difference should not be interpreted as a general performance advantage.

## Performance Analysis

The standalone simulator runs within a single process, while the multi-process simulator separates the UI, Core, and Logger into independent processes.

The multi-process design provides process separation and IPC-based communication, but process management and IPC can introduce additional overhead.

## Conclusion

The benchmark was successfully performed for the standalone and multi-process simulator. The functional IPC tests also confirmed that communication between UI, Core, and Logger works correctly.
