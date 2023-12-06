#ifndef TEXT_HPP
#define TEXT_HPP
#include <string>
#include <vector>
#include "lib/shape.hpp"
#include "lib/layer.hpp"
namespace game::RES
{
    class Text
    {
    public:
        Text();
        void setText(std::string text);
        void setPos(float x, float y);
        void setSize(float size);
        void draw(Layer *layer, bool clear = true);
        void setRatio(float ratio);

    private:
        std::string text;
        float x, y, size;
        float ratio = 2;
    };
}
#endif