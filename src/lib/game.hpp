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

    protected:
        void _start();
        virtual void tick() = 0;
        virtual void init() = 0;
        virtual void exit() = 0;
        void blockEvent(bool block);

        std::thread controllerThread;
        std::function<void()> callback;
        uint64_t TICK_DELAY = 16 * 1000 * 1000; // Nanoseconds
        bool quit = false;
    };
    extern std::map<int, game::RES::Layer> layers;
    extern game::EVENT::EventQueue eventQueue;
    extern game::RES::TextureManager textureManager;
    extern std::mutex layersMutex;
}
#endif