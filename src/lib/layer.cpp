#include "layer.hpp"

game::RES::Layer::Layer()
{
    name = "Unnamed Layer";
    shapes = std::vector<Shape>();
}

void game::RES::Layer::push_back(Shape shape)
{
    shapes.push_back(shape);
}