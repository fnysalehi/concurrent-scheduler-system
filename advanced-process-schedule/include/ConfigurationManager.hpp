#pragma once
#include <string>
#include "SchedulerTypes.hpp"
#include "ProcessQueue.hpp"

class ConfigurationManager {
public:
    static bool loadConfiguration(const std::string& configPath,
                                SchedulerType& memorySchedulerType,
                                SchedulerType& logicSchedulerType,
                                int& memoryQuantum,
                                int& logicQuantum,
                                ProcessQueue& memoryQueue,
                                ProcessQueue& logicQueue,
                                int* arrivalTimes);

private:
    static SchedulerType parseSchedulerType(const std::string& typeStr);
    static int parseQuantum(const std::string& str);
};