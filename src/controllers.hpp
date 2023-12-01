#ifndef CONTROLLERS_HPP
#define CONTROLLERS_HPP
#include "lib/game.hpp"
#include "lib/shape.hpp"
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
        float rotationSpeed = 1.0;

        const float MAX_HORIZ_COORD = 10;
        const float MAX_VERTI_COORD = 5;

        const float COLLISION_BOX_RADIUS = 0.7;
        const size_t ENERMY_SHIPS_IN_A_GEN = 10;

        int framesPerShipGeneration = 80;
        int framesSinceLastShipGeneration = 0;

        void generateEnermyShips();
        void generateAllyProjectile();

        void collisionDetection();

        bool outOfBound(game::RES::Shape &shape);
    };
}
#endif
