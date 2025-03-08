#include "SchedulerSystem.hpp"
#include <iostream>

int main() {
    SchedulerSystem scheduler;
    
    if (!scheduler.initialize(SchedulerSystem::DEFAULT_CONFIG_PATH)) {
        std::cerr << "Failed to initialize scheduler system\n";
        return 1;
    }
    
    scheduler.executeScheduling();
    return 0;
}