#include "text.hpp"
#include "lib/shape.hpp"
#include "lib/texture.hpp"
#include "lib/game.hpp"
#include "lib/layer.hpp"
#include <string>
#include <thread>
#include <mutex>

using namespace game::RES;

Text::Text()
{
    x = 0;
    y = 0;
    size = 1;
}

void Text::setText(std::string text)
{
    this->text = text;
}

void Text::setPos(float x, float y)
{
    this->x = x;
    this->y = y;
}

void Text::setSize(float size)
{
    this->size = size;
}

void Text::setRatio(float ratio)
{
    this->ratio = ratio;
}

void Text::draw(Layer *layer, bool clear)
{
    std::lock_guard<std::mutex> *layersLock = new std::lock_guard<std::mutex>(game::layersMutex);
    std::vector<Shape> &shapes = layer->shapes;
    if (clear)
        shapes.clear();
    float x = this->x;
    float y = this->y;
    float size = this->size;
    for (char c : text)
    {
        if (c == ' ')
        {
            x += size;
            continue;
        }
        std::string fileName = std::string(1, c);
        if (c == ':')
            fileName = "col";
        else if (c == '/')
            fileName = "fs";
        else if (c == '\\')
            fileName = "bs";
        if (!isalpha(c) || !islower(c))
        {
            fileName = "u" + fileName;
        }
        Shape shape;
        shape.texture = game::fontTextureManager.getTexture(fileName);
        shape.x = x;
        shape.y = y;
        shape.width = size;
        shape.height = size * ratio;
        shapes.push_back(shape);
        x += size;
    }
    delete layersLock;
}