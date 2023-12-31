#ifndef CONTROLLERS_HPP
#define CONTROLLERS_HPP
#include "lib/game.hpp"
#include "lib/shape.hpp"
#include "projectile.hpp"
#include "text.hpp"
#include "vec.hpp"
#include <json/json.h>
using game::vec::Vec2;
namespace game
{
    class StartScreenController : public Controller
    {
    public:
        StartScreenController() : Controller(){};

    private:
        void tick(); // Must be implemented
        void init(); // Must be implemented
        void exit(); // Must be implemented

        // User defined

        bool dir = true;
        int counter = 0;
    };

    class TestController : public Controller
    {
    public:
        TestController() : Controller(){};
        uint32_t getScore();

    private:
        void tick();
        void init();
        void exit();

        // User defined
        // CONSTANTS
        float rotationSpeed = 1.0;

        const float MAX_HORIZ_COORD = 10;
        const float MAX_VERTI_COORD = 5;

        const float COLLISION_BOX_RADIUS = 0.7;
        const size_t ENERMY_SHIPS_IN_A_GEN = 10;
        const size_t FRAMES_PER_ALLY_PROJECTILE_GEN = 10;
        const size_t FRAMES_PER_ENERMY_PROJECTILE_GEN = 30;

        int framesPerShipGeneration = 80;
        // VARIABLES
        Shape background;
        int framesSinceLastShipGeneration = 0;

        uint64_t frameCount = 0;

        Vec2 shipVelocity;

        uint32_t score = 0;
        int hp = 10;

        // Components:
        game::RES::Text scoreText;
        game::RES::Text hpText;

        // FUNCTIONS
        void calcVelocity();
        void moveBackground();

        void generateEnermyShips();
        void generateProjectiles();

        void collisionDetection();

        void hpVerification();

        bool outOfBound(game::RES::Shape &shape);

        game::projectile::ProjectileManager projectileManager;
    };

    class EndScreenController : public Controller
    {
    public:
        EndScreenController() : Controller(){};
        void setScore(uint32_t score);
        void setUserName(std::string userName);
        void submitScore();

    private:
        void tick();
        void init();
        void exit();

        // User defined
        uint32_t score = 0;
        std::string userName = "";

        Json::Value leaderboard;

        void renderLeaderboard();

        bool dir = true;
        int counter = 0;
    };
}
#endif
