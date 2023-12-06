#ifndef PERFORMANCE_HPP
#define PERFORMANCE_HPP
#include <chrono> // std::chrono::seconds
#include <thread> // std::this_thread::sleep_for
#include <string>
namespace game::PERF
{
    class PerformanceManager
    {
    public:
        uint32_t fps = 0;
        uint32_t ticks = 0;
        uint32_t renderLatency = 0;

        uint32_t r_fps = 0;
        uint32_t r_ticks = 0;
        uint32_t r_renderLatency = 0;

        uint64_t lastReport = 0;

        uint64_t startTimestamp = 0;

        bool quit = false;

        void start();
        void _start();
        void report();
        void cls();

        const std::string CLS = "\033[2J\033[1;1H";
        const std::string RED = "\033[31m";
        const std::string GREEN = "\033[32m";
        const std::string YELLOW = "\033[33m";
        const std::string DEFAULT = "\033[0m";
        const std::string BLUE = "\033[34m";
    };

    extern PerformanceManager performanceManager;
}
#endif