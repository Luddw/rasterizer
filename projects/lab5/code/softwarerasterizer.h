#pragma once
#include <GL/glew.h>

class Renderer
{
public:
	Renderer();
	~Renderer();

private:
	GLuint vb, ibo;
	unsigned int frame_buffer;
};


