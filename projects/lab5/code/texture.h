#pragma once
#include <GL/glew.h>
#include <vector>
#include <string>
#include "rasterdata.h"
#include "vec3.h"

class Texture
{
private:
	unsigned int tex_handle;
	std::string filepath;
	unsigned char* localbuf;
	int widht, height, bpp;
public:
	Texture(const std::string &path);
	Texture(unsigned char * buf);
	Texture();
	~Texture();

	void Bind(unsigned int textslot) const;
	void Unbind();

	int GetWidht() const { return widht;}
	int GetHeight() const { return height;}
	int Getbpp() const { return bpp;}
	void Update(unsigned width, unsigned height,void* data);
	Pixel GetColor(vec3& uvCoord);
};
