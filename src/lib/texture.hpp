#ifndef TEXTURE_HPP
#define TEXTURE_HPP
#include <GL/glut.h>
#include <string>
#include <map>
namespace game::RES
{
    struct Texture
    {
        Texture();
        ~Texture();
        Texture(std::string fileName);
        Texture(std::string fileName, float r, float g, float b);
        std::string fileName;
        int width;
        int height;
        bool hasAlpha;
        GLubyte **data;
        GLuint textureID;

    private:
        void initTexture();
        bool loadPngImage(bool overrideColor = false, uint8_t r = 0, uint8_t g = 0, uint8_t b = 0);
    };

    class TextureManager
    {
    public:
        TextureManager();
        ~TextureManager();
        Texture &getTexture(std::string fileName);
        void loadAllTextures(bool overrideColor = false, uint8_t r = 0, uint8_t g = 0, uint8_t b = 0);
        void changePath(std::string path);

    private:
        std::map<std::string, Texture> textures;
        std::string texturesPath = "res/";
    };
}
#endif