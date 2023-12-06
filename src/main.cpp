#include "lib/ui.hpp"
#include "lib/uiconf.hpp"
#include "lib/texture.hpp"
#include "lib/shape.hpp"
#include "lib/game.hpp"
#include "lib/layer.hpp"
#include "lib/performance.hpp"
#include "lib/utils.hpp"
#include "controllers.hpp"
#include <iostream>
#include <thread>

#include <GL/freeglut.h>

int main(int argc, char **argv, char **envp)
{
    bool isDev = game::UTILS::findInEnvp(envp, "GALAGA") && std::string(getenv("GALAGA")) == "dev";
    if (!isDev)
        game::UTILS::redirectCout("galaga.log");

    game::UI::UIConf ui_config;
    ui_config.windowTitle = "Test Game";
    ui_config.windowWidth = 1920;
    ui_config.windowHeight = 1080;

    game::UI::init(argc, argv, ui_config);

    game::textureManager.loadAllTextures();

    game::fontTextureManager.changePath("res/font1/");
    game::fontTextureManager.loadAllTextures(true, 255, 255, 255);

    game::StartScreenController controller;
    game::TestController controller2;
    game::EndScreenController controller3;

    controller.start()->then([&]()
                             {
        std::cout << "Controller CALLLLLED" << std::endl;
            // glutLeaveMainLoop();
        controller2.start(); });

    controller2.then([&]()
                     {
        std::cout << "Controller2 CALLLLLED" << std::endl;
            controller3.setScore(controller2.getScore());
            controller3.setUserName(getenv("LOGNAME"));
            controller3.start(); });

    controller3.then([&]()
                     {
            glutLeaveMainLoop(); });

    if (isDev)
        game::PERF::performanceManager.start();
    else
        glutFullScreen();

    game::UI::start();

    return 0;
}