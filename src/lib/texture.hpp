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

    class TextureManager
    {
    public:
        TextureManager();
        ~TextureManager();
        Texture &getTexture(std::string fileName);
        void loadAllTextures();

    private:
        std::map<std::string, Texture> textures;
        const std::string texturesPath = "res/";
    };
}
#endif