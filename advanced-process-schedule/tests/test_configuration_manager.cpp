#include <cassert>
#include <iostream>
#include "ConfigurationManager.hpp"

void testConfigurationLoading() {
    // Test setup
    SchedulerType memorySchedulerType;
    SchedulerType logicSchedulerType;
    int memoryQuantum = 0;
    int logicQuantum = 0;
    ProcessQueue memoryQueue;
    ProcessQueue logicQueue;
    int arrivalTimes[100] = {0};  // Initialize array with zeros

    // Test configuration loading
    bool result = ConfigurationManager::loadConfiguration(
        "../config/test.txt",
        memorySchedulerType,
        logicSchedulerType,
        memoryQuantum,
        logicQuantum,
        memoryQueue,
        logicQueue,
        arrivalTimes
    );

    // Verify results
    assert(result && "Configuration loading should succeed");
    assert(memorySchedulerType == SchedulerType::ROUND_ROBIN && "Memory scheduler should be RR");
    assert(logicSchedulerType == SchedulerType::HIGHEST_RESPONSE_RATIO && "Logic scheduler should be HRN");
    assert(memoryQuantum == 3 && "Memory quantum should be 3");
    assert(!memoryQueue.isEmpty() && "Memory queue should not be empty");
    assert(!logicQueue.isEmpty() && "Logic queue should not be empty");
    
    // Verify arrival times
    assert(arrivalTimes[1] == 0 && "P1 arrival time should be 0");
    assert(arrivalTimes[2] == 1 && "P2 arrival time should be 1");
    assert(arrivalTimes[3] == 1 && "P3 arrival time should be 1");
    assert(arrivalTimes[4] == 3 && "P4 arrival time should be 3");

    std::cout << "Configuration Manager tests passed successfully!\n";
}

int main() {
    testConfigurationLoading();
    return 0;
}