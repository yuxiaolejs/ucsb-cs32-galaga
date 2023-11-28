#include "utils.hpp"
#include <chrono>

uint32_t game::UTILS::getTimestamp()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}