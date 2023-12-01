
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
#include <cstdlib>

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
    // game::layers[0].shapes[2].x = (float)counter / 100.0;
    // game::layers[0].shapes[1].x = -(float)counter / 100.0;
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

    srand(game::UTILS::getTimestamp()%10000);
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

    // Continuous key press detection - press to move our ship
    auto &eqk = game::eventQueue.pressedKeys; // Shorten the stupid long name of a game variable
    auto &ourShip = game::layers[1].shapes[0];
    if (eqk.find('w') != eqk.end() && ourShip.y > -MAX_VERTI_COORD)
        game::layers[1].shapes[0].y -= 0.1;
    if (eqk.find('s') != eqk.end() && ourShip.y < MAX_VERTI_COORD)
        game::layers[1].shapes[0].y += 0.1;
    if (eqk.find('a') != eqk.end() && ourShip.x > -MAX_HORIZ_COORD)
        game::layers[1].shapes[0].x -= 0.1;
    if (eqk.find('d') != eqk.end() && ourShip.x < MAX_HORIZ_COORD)
        game::layers[1].shapes[0].x += 0.1;

    std::lock_guard<std::mutex> *layersLock = new std::lock_guard<std::mutex>(game::layersMutex);
    for (size_t i = 1; i < game::layers[1].shapes.size(); i++)
    {
        if (outOfBound(game::layers[1].shapes[i]))
        {
            game::layers[1].shapes.erase(game::layers[1].shapes.begin() + i);
        }
        game::layers[1].shapes[i].x -= 0.05;
    }
    delete layersLock;
    this->generateEnermyShips();
    this->collisionDetection();
    // game::layers[1].shapes[0].rotation -= rotationSpeed;
    // game::layers[1].shapes[0].x += 0.005;
}

void game::TestController::collisionDetection()
{
    auto &ourShip = game::layers[1].shapes[0];
    auto &shipVec = game::layers[1].shapes;
    for (size_t i = 1; i < game::layers[1].shapes.size(); i++)
    {
        if (abs(ourShip.x - shipVec[i].x) < COLLISION_BOX_RADIUS && abs(ourShip.y - shipVec[i].y) < COLLISION_BOX_RADIUS)
        {
            std::lock_guard<std::mutex> *layersLock = new std::lock_guard<std::mutex>(game::layersMutex);
            shipVec.erase(shipVec.begin() + i);
            quit = true;
            delete layersLock;
        }
    }
}

void game::TestController::generateEnermyShips()
{
    if (framesSinceLastShipGeneration < framesPerShipGeneration)
    {
        framesSinceLastShipGeneration++;
        return;
    }
    framesSinceLastShipGeneration = 0;

    game::RES::Shape shape;
    size_t shipsToGen = (rand() % ENERMY_SHIPS_IN_A_GEN)/2;
    for (size_t i = 0; i < shipsToGen; i++)
    {
        shape = game::RES::Shape();
        shape.texture = game::textureManager.getTexture("ship1"); // Enermy ship
        shape.x = 10;
        shape.y = (float)(rand() % 13) - ((float)ENERMY_SHIPS_IN_A_GEN / 2.0);
        shape.width = 1;
        shape.height = 1;
        shape.rotation = 90;

        game::layers[1].shapes.push_back(shape);
    }
}

void game::TestController::init()
{
    std::cout << "TestController init" << std::endl;
    game::RES::Shape shape;
    shape.texture = game::textureManager.getTexture("bg");
    shape.x = 0;
    shape.y = 0;
    shape.width = 20;
    shape.height = 11.5;

    game::layers.insert({1, game::RES::Layer()});
    game::layers[0].shapes.push_back(shape);

    game::layers.insert({0, game::RES::Layer()}); // Layer of ships

    shape.texture = game::textureManager.getTexture("ship1"); // Our ship
    shape.x = 0;
    shape.y = 0;
    shape.width = 1;
    shape.height = 1;
    shape.rotation = -90;

    game::layers[1].shapes.push_back(shape);
}

bool game::TestController::outOfBound(game::RES::Shape &shape)
{
    return shape.x < -MAX_HORIZ_COORD || shape.x > MAX_HORIZ_COORD || shape.y < -MAX_VERTI_COORD || shape.y > MAX_VERTI_COORD;
}

void game::TestController::exit()
{
    std::cout << "TestController exit" << std::endl;
    game::layers.clear();
}
