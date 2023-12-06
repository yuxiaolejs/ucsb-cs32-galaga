
#include <iostream> // std::cout, std::endl
#include <thread>   // std::this_thread::sleep_for
#include <chrono>   // std::chrono::seconds
#include <fstream>
#include "ui.hpp"
#include "game.hpp"
#include "utils.hpp"
#include "performance.hpp"

#include <GL/freeglut.h>
#include <mutex>
#include <thread>
namespace game
{
    std::map<int, game::RES::Layer> layers;
    game::EVENT::EventQueue eventQueue;
    game::RES::TextureManager textureManager;
    std::mutex layersMutex;
    game::RES::TextureManager fontTextureManager;
    std::ofstream logFile;
}

game::Controller::Controller()
{
}

game::Controller::Controller(std::function<void()> callback)
{
    this->callback = callback;
}

game::Controller *game::Controller::start()
{
    this->init();
    controllerThread = std::thread(&game::Controller::_start, this);
    return this;
}
game::Controller *game::Controller::then(std::function<void()> callback)
{
    this->callback = callback;
    return this;
}

void game::Controller::_start()
{
    while (true)
    {
        uint64_t tickStart = game::UTILS::highResTimestamp();
        if (quit)
        {
            this->exit();
            if (callback)
                callback();
            return;
        }
        this->tick();
        uint64_t tickDelay = game::UTILS::highResTimestamp() - tickStart;

        game::PERF::performanceManager.ticks++;
        game::PERF::performanceManager.renderLatency = tickDelay;

        if (tickDelay < TICK_DELAY)
            std::this_thread::sleep_for(std::chrono::nanoseconds(TICK_DELAY - tickDelay));
    }
}

void game::Controller::blockEvent(bool block)
{
    game::eventQueue.block = block;
}