#ifndef CONTROLLERS_HPP
#define CONTROLLERS_HPP
#include "game.hpp"
namespace game
{
    class TestController : public Controller
    {
    public:
        TestController() : Controller(){};
        void tick();

    private:
        float rotationSpeed = 1.0;
        void _Move(bool axis, bool dir);
    };
}
#endif
