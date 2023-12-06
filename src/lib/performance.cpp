#include "performance.hpp"
#include "utils.hpp"
#include <thread>
#include <iostream>
#include <iomanip>

game::PERF::PerformanceManager game::PERF::performanceManager;

void game::PERF::PerformanceManager::start()
{
    startTimestamp = game::UTILS::getTimestamp();
    std::thread(&game::PERF::PerformanceManager::_start, this).detach();
}

void game::PERF::PerformanceManager::_start()
{
    while (!quit)
    {
        cls();
        report();
        std::this_thread::sleep_for(std::chrono::milliseconds(60));
    }
}

void game::PERF::PerformanceManager::report()
{
    uint64_t now = game::UTILS::getTimestamp();
    std::cout << "+-----------------------+" << std::endl
              << "|   " << GREEN << "Game Performance" << DEFAULT << "    |" << std::endl
              << "+-----------------------+" << std::endl
              << "| " << BLUE << std::setw(18) << now << DEFAULT << "    |" << std::endl
              << "| " << YELLOW << std::setw(10) << "Runtime: "
              << std::setw(8) << (now - startTimestamp) << " ms" << DEFAULT << " |" << std::endl
              << "| " << YELLOW << std::setw(10) << "FPS: "
              << std::setw(8) << r_fps << " ps" << DEFAULT << " |" << std::endl
              << "| " << YELLOW << std::setw(10) << "Ticks: "
              << std::setw(8) << r_ticks << " ps" << DEFAULT << " |" << std::endl
              << "| " << YELLOW << std::setw(10) << "Latency: "
              << std::setw(8) << std::setprecision(6) << ((float)r_renderLatency / 1000.00) << " us" << DEFAULT << " |" << std::endl
              << "+-----------------------+" << std::endl;
    if (now - lastReport > 1000)
    {
        r_fps = fps;
        r_ticks = ticks;
        r_renderLatency = renderLatency;
        fps = 0;
        ticks = 0;
        renderLatency = 0;
        lastReport = now;
    }
}

void game::PERF::PerformanceManager::cls()
{
    std::cout << "\033[2J\033[1;1H";
}