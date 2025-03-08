#include "SchedulerSystem.hpp"
#include "ConfigurationManager.hpp"
#include "SchedulerAlgorithms.hpp"

SchedulerSystem::SchedulerSystem()
    : currentMemoryTime(0)
    , currentLogicTime(0)
    , memoryQuantum(0)
    , logicQuantum(0)
    , memorySchedulerType(SchedulerType::ROUND_ROBIN)
    , logicSchedulerType(SchedulerType::ROUND_ROBIN)
{
}

bool SchedulerSystem::initialize(const std::string& configPath) {
    return ConfigurationManager::loadConfiguration(
        configPath,
        memorySchedulerType,
        logicSchedulerType,
        memoryQuantum,
        logicQuantum,
        memoryQueue,
        logicQueue,
        arrivalTimes
    );
}

void SchedulerSystem::executeScheduling() {
    std::vector<std::thread> schedulerThreads;
    
    while (!memoryQueue.isEmpty() || !logicQueue.isEmpty()) {
        schedulerThreads.push_back(
            std::thread(&SchedulerSystem::scheduleProcesses, this)
        );
    }

    for (auto& thread : schedulerThreads) {
        thread.join();
    }
}

// Implement other methods...
