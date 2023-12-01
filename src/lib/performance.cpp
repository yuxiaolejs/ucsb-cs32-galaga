#include "performance.hpp"
#include <thread>
#include <iostream>
#include <iomanip>

game::PERF::PerformanceManager game::PERF::performanceManager;

void game::PERF::PerformanceManager::start()
{
    std::thread(&game::PERF::PerformanceManager::_start, this).detach();
}

void game::PERF::PerformanceManager::_start()
{
    while (!quit)
    {
        cls();
        report();
        std::this_thread::sleep_for(std::chrono::microseconds(1000));
    }
}

void game::PERF::PerformanceManager::report()
{
    std::cout << "+-----------------------+" << std::endl
              << "|   " << GREEN << "Game Performance" << DEFAULT << "    |" << std::endl
              << "+-----------------------+" << std::endl
              << "| " << YELLOW << std::setw(10) << "FPS: "
              << std::setw(8) << fps << " ps" << DEFAULT << " |" << std::endl
              << "| " << YELLOW << std::setw(10) << "Ticks: "
              << std::setw(8) << ticks << " ps" << DEFAULT << " |" << std::endl
              << "| " << YELLOW << std::setw(10) << "Latency: "
              << std::setw(8) << renderLatency << " ns" << DEFAULT << " |" << std::endl
              << "+-----------------------+" << std::endl;
}

void game::PERF::PerformanceManager::cls()
{
    std::cout << "\033[2J\033[1;1H";
}