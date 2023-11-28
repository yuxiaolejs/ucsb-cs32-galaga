
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
    std::cout << "Controller const" << std::endl;
}

game::Controller::Controller(std::function<void()> callback)
{
    std::cout << "Controller const" << std::endl;
    this->callback = callback;
}

game::Controller *game::Controller::start()
{
    std::cout << "Controller start" << std::endl;
    controllerThread = std::thread(&game::Controller::_start, this);
    return this;
}
game::Controller *game::Controller::then(std::function<void()> callback)
{
    std::cout << "Controller then" << std::endl;
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
            break;
        }

        this->tick();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / TICK_RATE));
    }
}

void game::Controller::tick()
{
    if (!game::UI::eventQueue.empty())
    {
        game::EVENT::Event event = game::UI::eventQueue.pop();
        if (event.data[0] == 'q')
        {
            // glutLeaveMainLoop();
            quit = true;
        }
        else if (event.data[0] == 'w')
        {
            _Move(true, true);
        }
        else if (event.data[0] == 's')
        {
            _Move(true, false);
        }
        else if (event.data[0] == 'a')
        {
            _Move(false, true);
        }
        else if (event.data[0] == 'd')
        {
            _Move(false, false);
        }
    }
    game::UI::layers[1].shapes[0].rotation -= rotationSpeed;
    game::UI::layers[1].shapes[0].x += 0.005;
}

void game::Controller::_Move(bool axis, bool dir)
{
    blockEvent(true);
    float *var;
    if (axis)
        var = &game::UI::layers[1].shapes[0].y;
    else
        var = &game::UI::layers[1].shapes[0].x;
    for (int i = 0; i < 10; i++)
    {
        if (dir)
            *var -= 0.1;
        else
            *var += 0.1;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    blockEvent(false);
}

void game::Controller::blockEvent(bool block)
{
    game::UI::eventQueue.block = block;
}