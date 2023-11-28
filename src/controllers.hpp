#ifndef CONTROLLERS_HPP
#define CONTROLLERS_HPP
#include "lib/game.hpp"
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
    };

    class TestController : public Controller
    {
    public:
        TestController() : Controller(){};

    private:
        void tick();
        void init();
        void exit();
        float rotationSpeed = 1.0;
        void _Move(bool axis, bool dir);
    };
}
#endif
