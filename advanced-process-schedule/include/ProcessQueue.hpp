#pragma once
#include <vector>
#include <mutex>
#include <thread>
#include "Process.hpp"

class ProcessQueue {
public:
    ProcessQueue() = default;
    
    void addProcess(const Process& process);
    void removeProcess(int processId);
    bool isEmpty() const;
    void sortByPriority();
    void sortByRemainingTime();
    void sortByResponseRatio(int currentTime, const int* arrivalTimes);
    
    std::vector<Process>& getProcesses() { return processes; }
    const std::vector<Process>& getProcesses() const { return processes; }

private:
    std::vector<Process> processes;
    mutable std::mutex queueMutex;
};
