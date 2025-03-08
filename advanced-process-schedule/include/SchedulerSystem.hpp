#pragma once
#include <mutex>
#include <vector>
#include <thread>
#include "ProcessQueue.hpp"
#include "SchedulerTypes.hpp"

class SchedulerSystem {
public:
    static constexpr const char* DEFAULT_CONFIG_PATH = "../config/test.txt";
    
    SchedulerSystem();
    bool initialize(const std::string& configPath);
    void executeScheduling();

private:
    ProcessQueue memoryQueue;
    ProcessQueue logicQueue;
    std::vector<int> memoryTimeline;
    std::vector<int> logicTimeline;
    int arrivalTimes[100];
    int memoryQuantum;
    int logicQuantum;
    SchedulerType memorySchedulerType;
    SchedulerType logicSchedulerType;
    int currentMemoryTime;
    int currentLogicTime;
    std::mutex schedulerMutex;

    void scheduleProcesses();
    void removeCompletedProcesses();
    void executeScheduler(QueueType queueType);
};
