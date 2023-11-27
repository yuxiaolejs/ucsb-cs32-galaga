#ifndef GAME_HPP
#define GAME_HPP
#include <thread>
namespace game
{
    struct Controller
    {
        Controller();
        void start();

    private:
        void _start();
        void tick();
        void blockEvent(bool block);
        std::thread controllerThread;
        const int TICK_RATE = 60;

        // User defined data
        float rotationSpeed = 1.0;

        void _Move(bool axis, bool dir);
    };
}
#endif