
/**
 * @file main.cpp
 * @brief Implementation of a pipe-based command chaining system
 * 
 * This program demonstrates IPC (Inter-Process Communication) using pipes
 * to chain multiple Unix commands together, similar to shell pipelines.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

// Command configuration
struct Command {
    const char* name;
    const char* arg;
};

// Pipeline configuration
const Command FIRST_COMMAND = {"ls", "/"};
const Command SECOND_COMMAND = {"wc", "-l"};

// Error handling function
void handle_error(const char* message) {
    perror(message);
    exit(EXIT_FAILURE);
}

// File descriptor management
void close_descriptors(int* descriptors, size_t count) {
    for (size_t i = 0; i < count; i++) {
        if (descriptors[i] != -1) {
            close(descriptors[i]);
        }
    }
}

int main() {
    int pipe_fd[2];
    int orig_stdin = -1, orig_stdout = -1;
    pid_t child_pid;
   
    // Save original stdin and stdout
    if ((orig_stdin = dup(STDIN_FILENO)) == -1) {
        handle_error("Failed to duplicate stdin");
    }
    
    if ((orig_stdout = dup(STDOUT_FILENO)) == -1) {
        close(orig_stdin);
        handle_error("Failed to duplicate stdout");
    }
    
    // Create pipe
    if (pipe(pipe_fd) == -1) {
        int descriptors[] = {orig_stdin, orig_stdout};
        close_descriptors(descriptors, 2);
        handle_error("Pipe creation failed");
    }
    
    // Fork process
    child_pid = fork();
    if (child_pid == -1) {
        int descriptors[] = {orig_stdin, orig_stdout, pipe_fd[0], pipe_fd[1]};
        close_descriptors(descriptors, 4);
        handle_error("Fork failed");
    }
    
    if (child_pid == 0) {  // Child process
        // Redirect stdout to pipe write end
        if (dup2(pipe_fd[1], STDOUT_FILENO) == -1) {
            handle_error("Failed to redirect stdout in child");
        }
        
        // Close unnecessary file descriptors
        int descriptors[] = {orig_stdout, orig_stdin, pipe_fd[0], pipe_fd[1]};
        close_descriptors(descriptors, 4);
        
        // Execute first command
        execlp(FIRST_COMMAND.name, FIRST_COMMAND.name, FIRST_COMMAND.arg, NULL);
        handle_error("Exec failed in child");
    } 
    else {  // Parent process
        // Redirect stdin to pipe read end
        if (dup2(pipe_fd[0], STDIN_FILENO) == -1) {
            handle_error("Failed to redirect stdin in parent");
        }
        
        // Restore original stdout
        if (dup2(orig_stdout, STDOUT_FILENO) == -1) {
            handle_error("Failed to restore stdout in parent");
        }

        // Close unnecessary file descriptors
        int descriptors[] = {pipe_fd[0], pipe_fd[1], orig_stdout, orig_stdin};
        close_descriptors(descriptors, 4);

        // Execute second command
        execlp(SECOND_COMMAND.name, SECOND_COMMAND.name, SECOND_COMMAND.arg, NULL);
        handle_error("Exec failed in parent");
    }
    
    return EXIT_SUCCESS;  // Never reached due to exec
}
