
#include <iostream> // std::cout, std::endl
#include <thread>   // std::this_thread::sleep_for
#include <chrono>   // std::chrono::seconds
#include "lib/ui.hpp"
#include "lib/game.hpp"
#include "lib/utils.hpp"
#include "controllers.hpp"

#include <GL/freeglut.h>
#include <mutex>
#include <thread>

void game::TestController::tick()
{
    if (!game::eventQueue.empty())
    {
        game::EVENT::Event event = game::eventQueue.pop();
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
    game::layers[1].shapes[0].rotation -= rotationSpeed;
    game::layers[1].shapes[0].x += 0.005;
}

void game::TestController::init()
{
    std::cout << "TestController init" << std::endl;
}

void game::TestController::exit()
{
    std::cout << "TestController exit" << std::endl;
}

void game::TestController::_Move(bool axis, bool dir)
{
    blockEvent(true);
    float *var;
    if (axis)
        var = &game::layers[1].shapes[0].y;
    else
        var = &game::layers[1].shapes[0].x;
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