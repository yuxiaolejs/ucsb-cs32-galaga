#ifndef UTILS_HPP
#define UTILS_HPP
#include <chrono>
#include <string>
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

    uint64_t getTimestamp();
    uint64_t highResTimestamp();

    bool findInEnvp(char **envp, std::string varToFind);

    void redirectCout(std::string filename);
}
#endif