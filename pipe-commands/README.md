# Command Pipeline System

A demonstration of Unix/Linux inter-process communication (IPC) using pipes to create command pipelines, similar to shell command chaining (e.g., `ls / | wc -l`).

## Features

- Robust error handling for all system calls
- Clean management of file descriptors
- Structured command configuration
- Process isolation through proper forking
- Standard I/O redirection

## Implementation Details

The system:
1. Creates a pipe for inter-process communication
2. Forks into two processes
3. Child process:
   - Executes the first command (`ls /`)
   - Redirects output to pipe
4. Parent process:
   - Executes the second command (`wc -l`)
   - Reads input from pipe
   - Outputs to original stdout

## Building

```bash
# Using gcc
gcc -Wall -Wextra -o pipeline main.cpp

# Using CMake
cmake ..
make
```

## Usage

Simply run the compiled executable:
```bash
./pipeline
```

The program will execute `ls /` and pipe its output to `wc -l`, displaying the total number of files in the root directory.

## Error Handling

The implementation includes comprehensive error handling for:
- Pipe creation
- Process forking
- File descriptor duplication
- Command execution
- Resource cleanup

## Dependencies

- POSIX-compliant operating system
- C++ compiler with C++11 support