#pragma once
#include <GL/glew.h>
#include <vector>
#include <string>
class Texture
{
private:
	unsigned int tex_handle;
	std::string filepath;
	unsigned char* localbuf;
	int widht, height, bpp;
public:
	Texture(const std::string &path);
	Texture();

	~Texture();

	void Bind(unsigned int textslot) const;
	void Unbind();

	int GetWidht() const { return widht;}
	int GetHeight() const { return height;}
	int Getbpp() const { return bpp;}

};
