#pragma once
#include <stdint.h>
#include <vector>
struct Color;
class Image
{
private:
    /* data */
public:
    Image(/* args */);
    Image(const char* filepath, std::vector<Color> &framebuffer, int width, int height);
    uint8_t* ProcessColor(std::vector<Color> &framebuffer);
    ~Image();
};

uint8_t* ProcessImage(std::vector<Color> &framebuffer);
void WriteImage(const char* filepath, std::vector<Color> &framebuffer, int width, int height);