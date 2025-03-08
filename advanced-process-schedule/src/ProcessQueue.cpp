#include "ProcessQueue.hpp"
#include <algorithm>
#include <mutex>

void ProcessQueue::addProcess(const Process& process) {
    std::lock_guard<std::mutex> lock(queueMutex);
    processes.push_back(process);
}

void ProcessQueue::removeProcess(int processId) {
    std::lock_guard<std::mutex> lock(queueMutex);
    processes.erase(
        std::remove_if(processes.begin(), processes.end(),
            [processId](const Process& p) { return p.processId == processId; }),
        processes.end()
    );
}

bool ProcessQueue::isEmpty() const {
    std::lock_guard<std::mutex> lock(queueMutex);
    return processes.empty();
}

void ProcessQueue::sortByPriority() {
    std::lock_guard<std::mutex> lock(queueMutex);
    std::sort(processes.begin(), processes.end(),
        [](const Process& left, const Process& right) {
            if (left.priority == right.priority)
                return left.processId < right.processId;
            return left.priority < right.priority;
        });
}

// Implement other sorting methods...
