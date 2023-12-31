
#include <iostream> // std::cout, std::endl
#include <thread>   // std::this_thread::sleep_for
#include <chrono>   // std::chrono::seconds
#include "lib/ui.hpp"
#include "lib/game.hpp"
#include "lib/utils.hpp"
#include "controllers.hpp"
#include "text.hpp"
#include "3rdparty/request.hpp"

#include <GL/freeglut.h>
#include <mutex>
#include <thread>
#include <cstdlib>
#include <json/json.h>
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

    game::layers.insert({301, game::RES::Layer()}); // Layer of text
    game::RES::Text text;
    text.setRatio(2.3);
    text.setSize(0.15);
    text.setPos(4.3, 5.4);
    text.setText(game::VERSION_NUMBER);
    text.draw(&game::layers[301], false);

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
    shipVelocity = Vec2(0.02, 0);
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
    // shipVelocity = shipVelocity - Vec2(0.02, 0);
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
    this->moveBackground();
    this->projectileManager.tick();
    // game::layers[1].shapes[0].rotation -= rotationSpeed;
    // game::layers[1].shapes[0].x += 0.005;
    frameCount++;
    this->score++;
    scoreText.setText("Score: " + std::to_string(score));
    scoreText.draw(&game::layers[301]);
    hpText.setText("HP: " + std::to_string(hp));
    hpText.draw(&game::layers[302]);
}

void game::TestController::moveBackground()
{
    if (game::layers[0].shapes[0].x < -20)
    {
        game::layers[0].shapes[0].x = 0;
        game::layers[0].shapes[1].x = 20;
    }
    game::layers[0].shapes[0].x -= 0.02;
    game::layers[0].shapes[1].x -= 0.02;
}

void game::TestController::collisionDetection()
{
    auto &ourShip = game::layers[1].shapes[0];
    auto &shipVec = game::layers[1].shapes;
    // Is our ship hit by enermy projectiles?
    bool ourShipHit = projectileManager.collisionDetection(ourShip, true);
    if (ourShipHit)
    {
        // quit = true;
        hp--;
        this->hpVerification();
        return;
    }
    for (size_t i = 1; i < game::layers[1].shapes.size(); i++)
    {
        // Is our ship hit by enermy ships?
        if (abs(ourShip.x - shipVec[i].x) < COLLISION_BOX_RADIUS && abs(ourShip.y - shipVec[i].y) < COLLISION_BOX_RADIUS)
        {
            std::lock_guard<std::mutex> *layersLock = new std::lock_guard<std::mutex>(game::layersMutex);
            shipVec.erase(shipVec.begin() + i);
            hp -= 3;
            this->hpVerification();
            // quit = true;
            delete layersLock;
        }
        // Is enermy ship hit by our projectiles?
        bool enermyShipHit = projectileManager.collisionDetection(shipVec[i], false);
        if (enermyShipHit)
        {
            std::lock_guard<std::mutex> *layersLock = new std::lock_guard<std::mutex>(game::layersMutex);
            shipVec.erase(shipVec.begin() + i);
            this->score += 1000;
            hp++;
            this->hpVerification();
            delete layersLock;
        }
    }
}

void game::TestController::hpVerification()
{
    if (hp <= 0)
        quit = true;
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
        shape.texture = game::textureManager.getTexture("ship_enemy_2"); // Enermy ship
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
        this->projectileManager.spawnStupidProjectile(game::vec::Vec2(shipVec[0].x, shipVec[0].y), true, shipVelocity / shipVelocity.magnitude() * 0.2);

    if (frameCount % FRAMES_PER_ENERMY_PROJECTILE_GEN == 0) // Generate enermy projectiles
        for (size_t i = 1; i < game::layers[1].shapes.size(); i++)
            this->projectileManager.spawnStupidProjectile(game::vec::Vec2(shipVec[i].x, shipVec[i].y), false);

    if (frameCount % 100 == 0 && game::layers[1].shapes.size() > 1) // Generate enermy projectiles
    {
        size_t i = rand() % (game::layers[1].shapes.size() - 1) + 1;
        this->projectileManager.spawnSmartProjectile(game::vec::Vec2(shipVec[i].x, shipVec[i].y), false, shipVec[0].id, &game::layers[1]);
    }
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
    this->background.x = 20;
    game::layers[0].shapes.push_back(this->background);

    game::layers.insert({1, game::RES::Layer()}); // Layer of ships

    game::RES::Shape shape;
    shape.texture = game::textureManager.getTexture("ship_friendly_1"); // Our ship
    shape.x = 0;
    shape.y = 0;
    shape.width = 1;
    shape.height = 1;
    shape.rotation = -90;

    game::layers[1].shapes.push_back(shape);

    game::layers.insert({201, game::RES::Layer()}); // Layer of projectiles (DO NOT TOUCH)

    this->projectileManager = game::projectile::ProjectileManager(&game::layers[201]);

    game::layers.insert({301, game::RES::Layer()}); // Layer of score
    game::layers.insert({302, game::RES::Layer()}); // Layer of hp

    scoreText.setRatio(2.3);
    scoreText.setSize(0.2);
    scoreText.setPos(-9.5, -5);

    hpText.setRatio(2.3);
    hpText.setSize(0.2);
    hpText.setPos(-9.5, -4.6);
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

uint32_t game::TestController::getScore()
{
    return this->score;
}

void game::EndScreenController::tick()
{
    if (!game::eventQueue.empty())
    {
        // One time key down detection
        game::EVENT::Event event = game::eventQueue.pop();
        if (event.type == game::EVENT::Event::EventType::KEYBOARD_EVENT && event.data[0] == 'q')
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

void game::EndScreenController::init()
{
    game::layers.insert({0, game::RES::Layer()}); // Layer of score
    game::RES::Shape shape;
    shape.texture = game::textureManager.getTexture("Gameover!");
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
    shape.texture = game::textureManager.getTexture("PRESS_Q");
    shape.x = 0;
    shape.y = 3.8;
    shape.width = 6;
    shape.height = 1;

    game::layers[0].shapes.push_back(shape);

    game::layers.insert({303, game::RES::Layer()}); // Layer of text

    game::RES::Text text;
    text.setRatio(2.3);
    text.setSize(0.15);
    text.setPos(4.3, 5.4);
    text.setText(game::VERSION_NUMBER);
    text.draw(&game::layers[303], false);

    game::layers.insert({301, game::RES::Layer()}); // Layer of score
    game::layers.insert({302, game::RES::Layer()}); // Layer of leaderboard

    game::RES::Text scoreText; // Render score
    scoreText.setRatio(2.3);
    scoreText.setSize(0.2);
    scoreText.setPos(4.8, -1);
    scoreText.setText("Your score:");
    scoreText.draw(&game::layers[301]);
    scoreText.setPos(5, 0);
    scoreText.setSize(0.6);
    scoreText.setText(std::to_string(score));
    scoreText.draw(&game::layers[301], false);

    game::RES::Text text2;
    text2.setRatio(2.3);
    text2.setSize(0.2);
    text2.setPos(-9.4, -4 - 0.2 * 2);
    text2.setText("Leaderboard Loading...");
    text2.draw(&game::layers[302], false);

    this->submitScore();
    this->leaderboard = game::HTTP::get(game::API_PREFIX + "/score");
    this->renderLeaderboard();
}

void game::EndScreenController::renderLeaderboard()
{
    const float leaderBoardFontSize = 0.15;
    const int maxNameLength = 12;
    const int maxScoreLength = 8;
    game::RES::Text text;
    text.setRatio(2.3);
    text.setSize(leaderBoardFontSize);
    text.setPos(-9.4, -4 - leaderBoardFontSize * 2);
    text.setText("Leaderboard:");
    text.draw(&game::layers[302], true);
    for (Json::Value::ArrayIndex i = 0; i < this->leaderboard.size(); i++)
    {
        std::string name = this->leaderboard[i].get("name", "N/A").asString();
        std::string time = this->leaderboard[i].get("time", "N/A").asString();
        std::string score = std::to_string(this->leaderboard[i].get("score", 0).asUInt());
        if (name.length() > maxNameLength)
            name = name.substr(0, maxNameLength);
        text.setPos(-9.4, -4 + (i + 1) * leaderBoardFontSize * 2);
        text.setText(name + " " + std::string(maxNameLength - name.length() + 1, '.') + " " +
                     score + std::string(maxScoreLength - score.length() + 1, ' ') + time);
        text.draw(&game::layers[302], false);
        if (i > 23)
            break;
    }
}

void game::EndScreenController::exit()
{
}

void game::EndScreenController::setScore(uint32_t score)
{
    this->score = score;
}

void game::EndScreenController::submitScore()
{
    Json::Value data;
    data["score"] = this->score;
    data["name"] = this->userName;
    game::HTTP::post(game::API_PREFIX + "/score", data);
}

void game::EndScreenController::setUserName(std::string userName)
{
    this->userName = userName;
}