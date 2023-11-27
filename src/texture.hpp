#ifndef TEXTURE_HPP
#define TEXTURE_HPP
#include <GL/glut.h>
#include <string>
namespace game::RES
{
    struct Texture
    {
        Texture();
        ~Texture();
        Texture(std::string fileName);
        std::string fileName;
        int width;
        int height;
        bool hasAlpha;
        GLubyte **data;
        GLuint textureID;

    private:
        void initTexture();
        bool loadPngImage();
    };
}
#endif