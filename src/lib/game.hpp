#ifndef GAME_HPP
#define GAME_HPP
#include <thread>
#include <functional>
#include <map>
#include <mutex>
#include <fstream>
#include "texture.hpp"
#include "layer.hpp"
#include "event.hpp"
#include "shape.hpp"

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
    extern game::RES::TextureManager fontTextureManager;
    extern std::mutex layersMutex;
    extern std::ofstream logFile;
    const std::string API_PREFIX = "https://cs32.tianleyu.com/galaga";
}
#endif