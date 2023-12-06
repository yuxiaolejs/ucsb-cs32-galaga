#include <GL/glut.h>
#include <string>
#include "shape.hpp"
#include "texture.hpp"
#include <png.h>
#include <iostream>
#include <cstring>

game::RES::Shape::Shape()
{
    this->width = 0;
    this->height = 0;
    this->x = 0;
    this->y = 0;
    this->texture = Texture();
    this->name = "noName";
    this->id = idCounter++;
    this->rotation = 0;
    this->opacity = 1;
    this->mode = GL_QUADS;
    this->vertices = std::vector<std::pair<GLfloat, GLfloat>>({{-1, -1}, {1, -1}, {1, 1}, {-1, 1}});
    this->texCoords = std::vector<std::pair<GLfloat, GLfloat>>({{0, 0}, {1, 0}, {1, 1}, {0, 1}});
}

std::vector<std::pair<GLfloat, GLfloat>> game::RES::Shape::getQuadsVertices()
{
    std::vector<std::pair<GLfloat, GLfloat>> quadsVertices;
    quadsVertices.push_back({this->x - this->width / 2, -this->y - this->height / 2});
    quadsVertices.push_back({this->x + this->width / 2, -this->y - this->height / 2});
    quadsVertices.push_back({this->x + this->width / 2, -this->y + this->height / 2});
    quadsVertices.push_back({this->x - this->width / 2, -this->y + this->height / 2});
    return quadsVertices;
}

u_int32_t game::RES::Shape::idCounter = 0;