#include "ui.hpp"
#include "uiconf.hpp"
#include "texture.hpp"
#include "shape.hpp"
#include "game.hpp"
#include "layer.hpp"
#include <iostream>
#include <thread>

#include <GL/freeglut.h>

int main(int argc, char **argv)
{
    game::UI::UIConf ui_config;
    ui_config.windowTitle = "Test Game";
    ui_config.windowWidth = 1920;
    ui_config.windowHeight = 1080;

    game::UI::init(argc, argv, ui_config);

    game::RES::Shape shape;
    game::RES::Texture texture2("res/logo.png");
    shape.texture = texture2;
    shape.x = 0;
    shape.y = 0;
    shape.width = 10;
    shape.height = 10;

    game::UI::layers.insert({1, game::RES::Layer()});
    game::UI::layers[0].shapes.push_back(shape);

    game::RES::Texture texture("res/alpha.png");
    std::cout << texture.textureID << std::endl;
    shape.texture = texture;
    shape.x = 0;
    shape.y = 0;
    shape.width = 2;
    shape.height = 2;

    game::UI::layers.insert({0, game::RES::Layer()});
    game::UI::layers[1].shapes.push_back(shape);

    game::Controller controller;
    game::Controller controller2;

    controller.start()->then([&]() {
        std::cout << "Controller CALLLLLED" << std::endl;
            // glutLeaveMainLoop();
            controller2.start();
    });

    controller2.then([]() {
        std::cout << "Controller2 CALLLLLED" << std::endl;
            glutLeaveMainLoop();
    });


    game::UI::start();

    return 0;
}