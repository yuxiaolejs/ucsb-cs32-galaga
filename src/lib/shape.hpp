#ifndef SHAPE_HPP
#define SHAPE_HPP
#include "texture.hpp"
#include <string>
#include <GL/glut.h>
#include <vector>
namespace game::RES
{
    class Shape
    {
    public:
        static u_int32_t idCounter;
        Shape();
        float width;
        float height;
        float x;
        float y;

        int rotation;
        float opacity;

        GLenum mode;
        std::vector<std::pair<GLfloat, GLfloat>> vertices;
        std::vector<std::pair<GLfloat, GLfloat>> texCoords;
        std::string name;
        u_int32_t id;
        Texture texture;
        std::vector<std::pair<GLfloat, GLfloat>> getQuadsVertices();
    };
}
#endif