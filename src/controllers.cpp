
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

void game::StartScreenController::tick()
{
    if (!game::eventQueue.empty())
    {
        // One time key down detection
        game::EVENT::Event event = game::eventQueue.pop();
        if (event.type == game::EVENT::Event::EventType::KEYBOARD_EVENT && event.data[0] == 'c')
            quit = true;
    }
    if (dir)
        counter++;
    else
        counter--;
    if (counter >= 100)
        dir = false;
    else if (counter <= 20)
        dir = true;
    game::layers[0].shapes[2].opacity = (float)counter / 100.0;
}
void game::StartScreenController::init()
{
    std::cout << "TestController init" << std::endl;

    game::layers.insert({1, game::RES::Layer()});

    game::RES::Shape shape;
    shape.texture = game::textureManager.getTexture("logo");
    shape.x = 0;
    shape.y = -1;
    shape.width = 8;
    shape.height = 8;

    game::layers[0].shapes.push_back(shape);

    shape = game::RES::Shape();
    shape.texture = game::textureManager.getTexture("LB_TEXT");
    shape.x = -6.8;
    shape.y = 5;
    shape.width = 6;
    shape.height = 1;

    game::layers[0].shapes.push_back(shape);

    shape = game::RES::Shape();
    shape.texture = game::textureManager.getTexture("PRESS_C");
    shape.x = 0;
    shape.y = 3.8;
    shape.width = 6;
    shape.height = 1;

    game::layers[0].shapes.push_back(shape);
}

void game::StartScreenController::exit()
{
    std::cout << "TestController exit" << std::endl;
    game::layers.clear();
}

void game::TestController::tick()
{
    if (!game::eventQueue.empty())
    {
        // One time key down detection
        game::EVENT::Event event = game::eventQueue.pop();
        if (event.type == game::EVENT::Event::EventType::KEYBOARD_EVENT && event.data[0] == 'q')
            quit = true;
    }
    // Continuous key press detection
    if (game::eventQueue.pressedKeys.find('w') != game::eventQueue.pressedKeys.end())
        _Move(true, true);
    else if (game::eventQueue.pressedKeys.find('s') != game::eventQueue.pressedKeys.end())
        _Move(true, false);
    else if (game::eventQueue.pressedKeys.find('a') != game::eventQueue.pressedKeys.end())
        _Move(false, true);
    else if (game::eventQueue.pressedKeys.find('d') != game::eventQueue.pressedKeys.end())
        _Move(false, false);

    game::layers[1].shapes[0].rotation -= rotationSpeed;
    game::layers[1].shapes[0].x += 0.005;
}

void game::TestController::init()
{
    std::cout << "TestController init" << std::endl;
    game::RES::Shape shape;
    shape.texture = game::textureManager.getTexture("logo");
    shape.x = 0;
    shape.y = 0;
    shape.width = 10;
    shape.height = 10;

    game::layers.insert({1, game::RES::Layer()});
    game::layers[0].shapes.push_back(shape);

    shape.texture = game::textureManager.getTexture("alpha");
    shape.x = 0;
    shape.y = 0;
    shape.width = 2;
    shape.height = 2;

    game::layers.insert({0, game::RES::Layer()});
    game::layers[1].shapes.push_back(shape);
}

void game::TestController::exit()
{
    std::cout << "TestController exit" << std::endl;
    game::layers.clear();
}

void game::TestController::_Move(bool axis, bool dir)
{
    blockEvent(true);
    float *var;
    if (axis)
        var = &game::layers[1].shapes[0].y;
    else
        var = &game::layers[1].shapes[0].x;
    for (int i = 0; i < 1; i++)
    {
        if (dir)
            *var -= 0.1;
        else
            *var += 0.1;
        // std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    blockEvent(false);
}