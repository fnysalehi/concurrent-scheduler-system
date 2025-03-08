#pragma once

enum class SchedulerType {
    ROUND_ROBIN = 1,
    SHORTEST_REMAINING_TIME = 2,
    HIGHEST_RESPONSE_RATIO = 3,
    MULTILEVEL = 4
};

enum class QueueType {
    MEMORY = 0,
    LOGIC = 1
};