#ifndef CONTROLLERS_HPP
#define CONTROLLERS_HPP
#include "lib/game.hpp"
#include "lib/shape.hpp"
#include "projectile.hpp"
#include "vec.hpp"
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

        // FUNCTIONS
        void calcVelocity();
        void moveBackground();

        void generateEnermyShips();
        void generateProjectiles();

        void collisionDetection();

        bool outOfBound(game::RES::Shape &shape);

        game::projectile::ProjectileManager projectileManager;
    };
}
#endif
