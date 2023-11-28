#ifndef UI_HPP
#define UI_HPP
#include <GL/glut.h>
#include <vector>
#include <queue>
#include <map>
#include <mutex>
#include "shape.hpp"
#include "uiconf.hpp"
#include "layer.hpp"
#include "event.hpp"

namespace game::UI
{
    // extern std::map<int, game::RES::Layer> layers;
    // extern game::EVENT::EventQueue eventQueue;

    void start();

    int init(int argc, char **argv, UIConf conf = UIConf());
    void idle();
    void reshape(int w, int h);
    void glInit();
    void display();
    void keyboard(unsigned char key, int x, int y);
}
#endif