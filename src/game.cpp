
#include <iostream> // std::cout, std::endl
#include <thread>   // std::this_thread::sleep_for
#include <chrono>   // std::chrono::seconds
#include "ui.hpp"
#include "game.hpp"
#include "utils.hpp"

#include <GL/freeglut.h>
#include <mutex>
#include <thread>

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
        if (quit)
        {
            if (callback)
                callback();
            this->exit();
            break;
        }

        this->tick();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / TICK_RATE));
    }
}

void game::Controller::blockEvent(bool block)
{
    game::UI::eventQueue.block = block;
}