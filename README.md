# Operating Systems Concepts Implementation

An ongoing collection of practical implementations demonstrating fundamental operating system concepts and mechanisms. Each implementation serves as a working example of core OS principles, focusing on system programming and concurrent operations.

## Projects

### 1. Command Pipeline System (`pipe-commands/`)

A demonstration of inter-process communication (IPC) using pipes to create command pipelines, similar to shell command chaining (e.g., `ls / | wc -l`). This implementation showcases:

- Process creation and management
- File descriptor handling
- Inter-process communication using pipes
- System call error handling

[More details](pipe-commands/README.md)

### 2. Process Scheduler (`process-scheduler/`)

A concurrent process scheduling system implementing multiple scheduling algorithms for both memory and logic processes. Features include:

- Multiple scheduling algorithms (RR, SRT, HRRN)
- Dual queue management
- Priority-based processing
- Thread-safe operations

[More details](process-scheduler/README.md)
