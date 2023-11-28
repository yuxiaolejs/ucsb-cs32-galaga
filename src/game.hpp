#ifndef GAME_HPP
#define GAME_HPP
#include <thread>
#include <functional>
namespace game
{
    class Controller
    {
    public:
        Controller();
        Controller(std::function<void()> callback);
        Controller *start();
        Controller *then(std::function<void()> callback);

    private:
        void _start();
        void tick();
        void blockEvent(bool block);
        std::thread controllerThread;
        const int TICK_RATE = 60;
        std::function<void()> callback;

        bool quit = false;

        // User defined data
        float rotationSpeed = 1.0;

        void _Move(bool axis, bool dir);
    };
}
#endif