# Concurrent Scheduler System

A concurrent process scheduling system implementing multiple scheduling algorithms for both memory and logic processes. The system demonstrates concepts in operating systems and concurrent programming.

## Features

- **Dual Queue Management**: Separate handling of memory and logic processes
- **Multiple Scheduling Algorithms**:
  - Round Robin (RR)
  - Shortest Remaining Time (SRT)
  - Highest Response Ratio Next (HRRN)
  - Multilevel Queue Scheduling
- **Concurrent Execution**: Thread-based implementation for parallel process handling
- **Priority-based Processing**: Support for process priorities and quantum-based execution

## Performance Considerations

- Thread-safe operations using mutex locks
- Efficient process queue management
- Optimized scheduling algorithms

