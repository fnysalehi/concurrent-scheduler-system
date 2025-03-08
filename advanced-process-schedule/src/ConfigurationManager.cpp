#include "ConfigurationManager.hpp"
#include <fstream>
#include <sstream>
#include <string>

bool ConfigurationManager::loadConfiguration(
    const std::string& configPath,
    SchedulerType& memorySchedulerType,
    SchedulerType& logicSchedulerType,
    int& memoryQuantum,
    int& logicQuantum,
    ProcessQueue& memoryQueue,
    ProcessQueue& logicQueue,
    int* arrivalTimes
) {
    std::ifstream configFile(configPath);
    if (!configFile.is_open()) {
        return false;
    }

    std::string line;
    
    // Read memory scheduler configuration
    std::getline(configFile, line);
    if (line.find("Memory:") != 0) {
        return false;
    }
    memorySchedulerType = parseSchedulerType(line.substr(8));
    if (memorySchedulerType == SchedulerType::ROUND_ROBIN || 
        memorySchedulerType == SchedulerType::SHORTEST_REMAINING_TIME) {
        memoryQuantum = parseQuantum(line);
    }

    // Read logic scheduler configuration
    std::getline(configFile, line);
    if (line.find("Logic:") != 0) {
        return false;
    }
    logicSchedulerType = parseSchedulerType(line.substr(7));
    if (logicSchedulerType == SchedulerType::ROUND_ROBIN || 
        logicSchedulerType == SchedulerType::SHORTEST_REMAINING_TIME) {
        logicQuantum = parseQuantum(line);
    }

    // Read number of processes
    std::getline(configFile, line);
    int processCount = std::stoi(line);

    // Read process configurations
    for (int i = 0; i < processCount; i++) {
        std::getline(configFile, line);
        std::istringstream iss(line);
        std::string processId;
        int arrivalTime;

        iss >> processId;  // Read "P1:", "P2:", etc.
        processId = processId.substr(1, processId.length() - 2);  // Remove "P" and ":"
        iss >> arrivalTime;
        arrivalTimes[std::stoi(processId)] = arrivalTime;

        std::string burst;
        while (iss >> burst) {
            Process process;
            process.processId = std::stoi(processId);
            process.type = (burst[0] == 'M') ? ProcessType::MEMORY : ProcessType::LOGIC;
            process.burstTime = burst[1] - '0';
            process.remainingTime = process.burstTime;
            process.priority = 1;  // Default priority

            if (process.type == ProcessType::MEMORY) {
                memoryQueue.addProcess(process);
            } else {
                logicQueue.addProcess(process);
            }
        }
    }

    configFile.close();
    return true;
}

SchedulerType ConfigurationManager::parseSchedulerType(const std::string& typeStr) {
    if (typeStr.find("RR") == 0) {
        return SchedulerType::ROUND_ROBIN;
    } else if (typeStr.find("SRT") == 0) {
        return SchedulerType::SHORTEST_REMAINING_TIME;
    } else if (typeStr.find("HRN") == 0) {
        return SchedulerType::HIGHEST_RESPONSE_RATIO;
    } else if (typeStr.find("ML") == 0) {
        return SchedulerType::MULTILEVEL;
    }
    return SchedulerType::ROUND_ROBIN;  // Default
}

int ConfigurationManager::parseQuantum(const std::string& str) {
    size_t pos = str.find_last_of(' ');
    if (pos != std::string::npos) {
        return std::stoi(str.substr(pos + 1));
    }
    return 1;  // Default quantum
}