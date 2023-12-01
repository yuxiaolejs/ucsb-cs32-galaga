#ifndef LAYER_HPP
#define LAYER_HPP
#include "shape.hpp"
#include <string>
#include <GL/glut.h>
#include <vector>
namespace game::RES
{
    struct Layer
    {
        Layer();
        void push_back(Shape shape);
        std::string name;
        std::vector<Shape> shapes;
    };
}
#endif