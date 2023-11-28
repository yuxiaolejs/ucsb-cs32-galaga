#ifndef CONTROLLERS_HPP
#define CONTROLLERS_HPP
#include "lib/game.hpp"
namespace game
{
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
