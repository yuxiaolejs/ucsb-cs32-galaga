#ifndef UTILS_HPP
#define UTILS_HPP
#include <chrono>
namespace game::UTILS
{
    class Timer
    {
    public:
        Timer();
        void start();
        void stop();
        void reset();
        double getElapsedTime();
        bool isRunning();

    private:
        std::chrono::time_point<std::chrono::system_clock> startTime;
        std::chrono::time_point<std::chrono::system_clock> endTime;
        bool running;
    };

    uint32_t getTimestamp();
}
#endif