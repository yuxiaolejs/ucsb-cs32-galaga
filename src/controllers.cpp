
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
#include "vec.hpp"
using game::vec::Vec2;
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

    srand(game::UTILS::getTimestamp() % 10000);
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
    shipVelocity = Vec2(0.1, 0);
    if (eqk.find('w') != eqk.end() && ourShip.y > -MAX_VERTI_COORD)
    {
        game::layers[1].shapes[0].y -= 0.1;
        shipVelocity.y -= 0.1;
    }
    if (eqk.find('s') != eqk.end() && ourShip.y < MAX_VERTI_COORD)
    {
        game::layers[1].shapes[0].y += 0.1;
        shipVelocity.y += 0.1;
    }
    if (eqk.find('a') != eqk.end() && ourShip.x > -MAX_HORIZ_COORD)
    {
        game::layers[1].shapes[0].x -= 0.1;
        shipVelocity.x -= 0.1;
    }
    if (eqk.find('d') != eqk.end() && ourShip.x < MAX_HORIZ_COORD)
    {
        game::layers[1].shapes[0].x += 0.1;
        shipVelocity.x += 0.1;
    }
    game::layers[1].shapes[0].rotation = shipVelocity.getAngleDeg() - 90;
    // Move all enermy ships
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
    this->generateProjectiles();
    this->collisionDetection();
    this->projectileManager.tick();
    // game::layers[1].shapes[0].rotation -= rotationSpeed;
    // game::layers[1].shapes[0].x += 0.005;
    frameCount++;
}

void game::TestController::collisionDetection()
{
    auto &ourShip = game::layers[1].shapes[0];
    auto &shipVec = game::layers[1].shapes;
    // Is our ship hit by enermy projectiles?
    bool ourShipHit = projectileManager.collisionDetection(ourShip, true);
    if (ourShipHit)
    {
        quit = true;
        return;
    }
    for (size_t i = 1; i < game::layers[1].shapes.size(); i++)
    {
        // Is our ship hit by enermy ships?
        if (abs(ourShip.x - shipVec[i].x) < COLLISION_BOX_RADIUS && abs(ourShip.y - shipVec[i].y) < COLLISION_BOX_RADIUS)
        {
            std::lock_guard<std::mutex> *layersLock = new std::lock_guard<std::mutex>(game::layersMutex);
            shipVec.erase(shipVec.begin() + i);
            quit = true;
            delete layersLock;
        }
        // Is enermy ship hit by our projectiles?
        bool enermyShipHit = projectileManager.collisionDetection(shipVec[i], false);
        if (enermyShipHit)
        {
            std::lock_guard<std::mutex> *layersLock = new std::lock_guard<std::mutex>(game::layersMutex);
            shipVec.erase(shipVec.begin() + i);
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
    size_t shipsToGen = (rand() % ENERMY_SHIPS_IN_A_GEN) / 2;
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

void game::TestController::generateProjectiles()
{
    auto &shipVec = game::layers[1].shapes;
    if (frameCount % FRAMES_PER_ALLY_PROJECTILE_GEN == 0) // Generate ally projectiles
        this->projectileManager.spawnStupidProjectile(game::vec::Vec2(shipVec[0].x, shipVec[0].y), true, shipVelocity);

    if (frameCount % FRAMES_PER_ENERMY_PROJECTILE_GEN == 0) // Generate enermy projectiles
        for (size_t i = 1; i < game::layers[1].shapes.size(); i++)
            this->projectileManager.spawnStupidProjectile(game::vec::Vec2(shipVec[i].x, shipVec[i].y), false);
}

void game::TestController::init()
{
    std::cout << "TestController init" << std::endl;
    this->background.texture = game::textureManager.getTexture("bg");
    this->background.x = 0;
    this->background.y = 0;
    this->background.width = 20;
    this->background.height = 11.5;

    game::layers.insert({0, game::RES::Layer()});
    game::layers[0].shapes.push_back(this->background);

    game::layers.insert({1, game::RES::Layer()}); // Layer of ships

    game::RES::Shape shape;
    shape.texture = game::textureManager.getTexture("ship1"); // Our ship
    shape.x = 0;
    shape.y = 0;
    shape.width = 1;
    shape.height = 1;
    shape.rotation = -90;

    game::layers[1].shapes.push_back(shape);

    game::layers.insert({201, game::RES::Layer()}); // Layer of projectiles (DO NOT TOUCH)

    this->projectileManager = game::projectile::ProjectileManager(&game::layers[201]);
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
