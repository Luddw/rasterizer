#include "image.h"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"
#include "color.h"

Image::Image(const char* filepath, std::vector<Color> &framebuffer, int width, int height)
{
    uint8_t *buf = ProcessColor(framebuffer);
    stbi_flip_vertically_on_write(1);
    bool result = stbi_write_png(filepath, width, height, Color::channels, buf, width * Color::channels);
    delete[] buf;
}

Image::Image(/* args */)
{

}

Image::~Image()
{
}

uint8_t* Image::ProcessColor(std::vector<Color> &framebuffer)
{
    uint8_t *pix = new uint8_t[framebuffer.size() * Color::channels];
    int pixel_index = 0;
    for (size_t i = 0; i < framebuffer.size(); i++)
    {
        uint8_t i_red   = static_cast<uint8_t>(255.99f * framebuffer[i].r); // i-th color red
        uint8_t i_green = static_cast<uint8_t>(255.99f * framebuffer[i].g); // i-th color green
        uint8_t i_blue  = static_cast<uint8_t>(255.99f * framebuffer[i].b); // i-th color 
        pix[pixel_index++] = i_red;
        pix[pixel_index++] = i_green;
        pix[pixel_index++] = i_blue;
    }
    
    return pix;
}

uint8_t* ProcessImage(std::vector<Color> &framebuffer)
{
    uint8_t *pix = new uint8_t[framebuffer.size() * Color::channels];
    int pixel_index = 0;
    for (size_t i = 0; i < framebuffer.size(); i++)
    {
        uint8_t i_red   = static_cast<uint8_t>(255.99f * framebuffer[i].r); // i-th color red
        uint8_t i_green = static_cast<uint8_t>(255.99f * framebuffer[i].g); // i-th color green
        uint8_t i_blue  = static_cast<uint8_t>(255.99f * framebuffer[i].b); // i-th color 
        pix[pixel_index++] = i_red;
        pix[pixel_index++] = i_green;
        pix[pixel_index++] = i_blue;
    }
    
    return pix;
}

void WriteImage(const char* filepath, std::vector<Color> &framebuffer, int width, int height)
{
    uint8_t *buf = ProcessImage(framebuffer);
    stbi_flip_vertically_on_write(1);
    bool result = stbi_write_png(filepath, width, height, Color::channels, buf, width * Color::channels);
    delete[] buf;
}