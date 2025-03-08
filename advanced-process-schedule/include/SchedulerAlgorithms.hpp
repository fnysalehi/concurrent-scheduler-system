#pragma once
#include "ProcessQueue.hpp"
#include "SchedulerTypes.hpp"

class SchedulerAlgorithms {
public:
    static void executeRoundRobin(ProcessQueue& queue, int quantum, int& currentTime, std::vector<int>& timeline);
    static void executeSRT(ProcessQueue& queue, int quantum, int& currentTime, std::vector<int>& timeline);
    static void executeHRRN(ProcessQueue& queue, int& currentTime, const int* arrivalTimes, std::vector<int>& timeline);

private:
    static int getNextProcess(const ProcessQueue& queue, int currentTime, const int* arrivalTimes);
};