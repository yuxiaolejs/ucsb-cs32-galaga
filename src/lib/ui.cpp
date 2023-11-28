#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <png.h>
#include <iostream>
#include <cstring>
#include <map>
#include <vector>
#include <mutex>

#include "ui.hpp"
#include "uiconf.hpp"
#include "texture.hpp"
#include "shape.hpp"
#include "utils.hpp"
#include "layer.hpp"
#include "game.hpp"

namespace game::UI
{
    u_int32_t lastFpsReport = 0;
    u_int32_t frames = 0;

    UIConf conf;
}

int frameCount = 0;

void game::UI::idle()
{
    u_int32_t now = game::UTILS::getTimestamp();
    if (now - lastFpsReport > 1000)
    {
        std::cout << "FPS: " << frames << std::endl;
        lastFpsReport = now;
        frames = 0;
    }
    frames++;
    // std::cout << "Idle, Frame Count:" << frames << std::endl;
    glutPostRedisplay();
}

void game::UI::reshape(int w, int h)
{
    if (h == 0)
        h = 1;
    float ratio = (float)w / (float)h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    ratio = 16.0 / 9.0;
    gluOrtho2D(-conf.canvasSize, conf.canvasSize, -conf.canvasSize / ratio, conf.canvasSize / ratio);

    // if (w <= h)
    //     gluOrtho2D(-conf.canvasSize, conf.canvasSize, -conf.canvasSize / ratio, conf.canvasSize / ratio);
    // else
    //     gluOrtho2D(-conf.canvasSize * ratio, conf.canvasSize * ratio, -conf.canvasSize, conf.canvasSize);

    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, w, h);
}

void game::UI::glInit()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float ratio = 16.0 / 9.0;
    gluOrtho2D(-conf.canvasSize, conf.canvasSize, -conf.canvasSize / ratio, conf.canvasSize / ratio);
}

void game::UI::display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Rotate the square
    // glRotatef(angle, 0.0, 0.0, 1.0);

    // Enable and bind the texture

    for (auto layer : game::layers)
    {
        std::vector<game::RES::Shape> shapes = layer.second.shapes;
        for (size_t i = 0; i < shapes.size(); i++)
        {
            glPushMatrix();
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, shapes[i].texture.textureID);

            // Draw a textured square
            float trans_x = shapes[i].x, trans_y = -shapes[i].y;

            glTranslatef(trans_x, trans_y, 0.0);
            glRotatef(shapes[i].rotation, 0.0, 0.0, 1.0);
            glTranslatef(-trans_x, -trans_y, 0.0);

            glBegin(shapes[i].mode);
            if (shapes[i].mode == GL_QUADS)
                shapes[i].vertices = shapes[i].getQuadsVertices();

            for (size_t j = 0; j < shapes[i].vertices.size(); j++)
            {
                glTexCoord2f(shapes[i].texCoords[j].first, shapes[i].texCoords[j].second);
                glVertex2f(shapes[i].vertices[j].first, shapes[i].vertices[j].second);
            }
            glEnd();

            glBindTexture(GL_TEXTURE_2D, 0);
            glDisable(GL_TEXTURE_2D);
            glPopMatrix();
        }
    }
    glutSwapBuffers();
}

void game::UI::keyboard(unsigned char key, int x, int y)
{
    std::cout << "Key pressed: " << key << std::endl;
    game::EVENT::Event event;
    event.type = game::EVENT::Event::EventType::KEYBOARD_EVENT;
    event.data.push_back(key);
    eventQueue.push(event);
}

void game::UI::keyboardUp(unsigned char key, int x, int y)
{
    std::cout << "Key released: " << key << std::endl;
    game::EVENT::Event event;
    event.type = game::EVENT::Event::EventType::KEYBOARD_UP_EVENT;
    event.data.push_back(key);
    eventQueue.push(event);
}

int game::UI::init(int argc, char **argv, UIConf config)
{
    // Initialize texture data here
    // textureImage = ...
    // loadPngImage(filename, textureWidth, textureHeight, hasAlpha, &textureImage);
    conf = config;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(config.windowPosX, config.windowPosY);
    glutInitWindowSize(config.windowWidth, config.windowHeight);
    glutCreateWindow(config.windowTitle.c_str());

    if (config.enableAlpha)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    std::cout << "Rendering complete." << std::endl;

    return 0;
}

void game::UI::start()
{
    glInit();
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutIgnoreKeyRepeat(1);
    glutKeyboardUpFunc(keyboardUp);
    glutMainLoop();
}