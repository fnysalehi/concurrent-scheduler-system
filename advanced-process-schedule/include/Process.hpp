#pragma once

enum class ProcessType {
    MEMORY = 0,
    LOGIC = 1
};

struct Process {
    ProcessType type;
    int processId;
    int priority;
    int burstTime;
    int remainingTime;

    bool operator==(const Process& other) const {
        return processId == other.processId;
    }
};