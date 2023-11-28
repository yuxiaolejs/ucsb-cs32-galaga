#include <GL/glut.h>
#include <string>
#include "texture.hpp"
#include <png.h>
#include <iostream>
#include <cstring>

game::RES::Texture::Texture()
{
    this->fileName = "";
    this->width = 0;
    this->height = 0;
    this->hasAlpha = false;
    this->data = nullptr;
    this->textureID = 0;
}
game::RES::Texture::~Texture()
{
}
game::RES::Texture::Texture(std::string fileName)
{
    this->width = 0;
    this->height = 0;
    this->hasAlpha = false;
    this->textureID = 0;

    this->data = new GLubyte *; // Init data pointer, possible memory leak

    this->fileName = fileName;
    if (!this->loadPngImage())
    {
        std::cerr << "Error reading texture: " << fileName << std::endl;
        this->textureID = 0;
        return;
    }
    this->initTexture();
    if (!glIsTexture(this->textureID))
    {
        std::cerr << "Error loading texture: " << fileName << std::endl;
        this->textureID = 0;
        free(*this->data);
        delete this->data;
        return;
    }
    free(*this->data);
    delete this->data;
}
void game::RES::Texture::initTexture()
{
    glGenTextures(1, &(this->textureID));
    glBindTexture(GL_TEXTURE_2D, this->textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if (this->hasAlpha)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, *this->data);
    else
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, *this->data);
}
bool game::RES::Texture::loadPngImage()
{
    png_structp png_ptr;
    png_infop info_ptr;
    unsigned int sig_read = 0;
    int color_type, interlace_type;
    FILE *fp;

    if ((fp = fopen(this->fileName.c_str(), "rb")) == NULL)
        return false;

    /* Create and initialize the png_struct
     * with the desired error handler
     * functions.  If you want to use the
     * default stderr and longjump method,
     * you can supply NULL for the last
     * three parameters.  We also supply the
     * the compiler header file version, so
     * that we know if the application
     * was compiled with a compatible version
     * of the library.  REQUIRED
     */
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
                                     NULL, NULL, NULL);

    if (png_ptr == NULL)
    {
        fclose(fp);
        return false;
    }

    /* Allocate/initialize the memory
     * for image information.  REQUIRED. */
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL)
    {
        fclose(fp);
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return false;
    }

    /* Set error handling if you are
     * using the setjmp/longjmp method
     * (this is the normal method of
     * doing things with libpng).
     * REQUIRED unless you  set up
     * your own error handlers in
     * the png_create_read_struct()
     * earlier.
     */
    if (setjmp(png_jmpbuf(png_ptr)))
    {
        /* Free all of the memory associated
         * with the png_ptr and info_ptr */
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        /* If we get here, we had a
         * problem reading the file */
        return false;
    }

    /* Set up the output control if
     * you are using standard C streams */
    png_init_io(png_ptr, fp);

    /* If we have already
     * read some of the signature */
    png_set_sig_bytes(png_ptr, sig_read);

    /*
     * If you have enough memory to read
     * in the entire image at once, and
     * you need to specify only
     * transforms that can be controlled
     * with one of the PNG_TRANSFORM_*
     * bits (this presently excludes
     * dithering, filling, setting
     * background, and doing gamma
     * adjustment), then you can read the
     * entire image (including pixels)
     * into the info structure with this
     * call
     *
     * PNG_TRANSFORM_STRIP_16 |
     * PNG_TRANSFORM_PACKING  forces 8 bit
     * PNG_TRANSFORM_EXPAND forces to
     *  expand a palette into RGB
     */
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);

    png_uint_32 _width, _height;
    int bit_depth;
    png_get_IHDR(png_ptr, info_ptr, &_width, &_height, &bit_depth, &color_type,
                 &interlace_type, NULL, NULL);
    width = _width;
    height = _height;

    if (color_type & PNG_COLOR_MASK_ALPHA)
    {
        hasAlpha = true;
    }

    unsigned int row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    std::cout << "Memalloc: " << row_bytes * height << std::endl;
    *data = (unsigned char *)malloc(row_bytes * height);

    png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);

    for (int i = 0; i < height; i++)
    {
        // note that png is ordered top to
        // bottom, but OpenGL expect it bottom to top
        // so the order or swapped
        memcpy(*data + (row_bytes * (height - 1 - i)), row_pointers[i], row_bytes);
    }

    /* Clean up after the read,
     * and free any memory allocated */
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    /* Close the file */
    fclose(fp);

    /* That's it */
    return true;
}

game::RES::TextureManager::TextureManager()
{
}
game::RES::TextureManager::~TextureManager()
{
}
game::RES::Texture &game::RES::TextureManager::getTexture(std::string fileName)
{
    if (this->textures.find(fileName) == this->textures.end())
    {
        this->textures.insert({fileName, Texture(texturesPath + fileName + ".png")});
    }
    return this->textures[fileName];
}