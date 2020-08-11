#pragma once
#include <GL/glew.h>
#include <map>
#include <iterator>
#include "matlib.h"
#include <vector>

struct Vertex
{
	Vector4D pos;
	Vector4D color;
	Vector4D normal;
};

struct BufferObject
{
	
	std::vector<Vertex> v_buffer;
	std::vector<unsigned int> i_buffer;

	BufferObject(std::vector<Vertex> v_buff, std::vector<unsigned int> i_buff)
	{
		this->v_buffer = v_buff;
		this->i_buffer = i_buff;
	};
};		

struct Color
{
	char r;
	char g;
	char b;
	char a;
};

class Renderer
{
public:
	Renderer();
	Renderer(const int widht, const int height);
	~Renderer();
	void AddVertexBuffer(Vertex* buffer);
	void AddIndexBuffer(unsigned int* buffer);
	const unsigned int AddBuffer(std::vector<Vertex> vbuff, std::vector<unsigned int> ibuff);
	void SetupFrameBuffer(int widht, int height);
	unsigned int* GetFramebuffer();
	const int GetFramebufferSize();
	void PlacePixel(int x, int y, Color color);
	void SetVertextShader();
	void SetFragmentShader();
	void RasterizeTriangle(Vertex x, Vertex y, Vertex z);
private:
	//GLuint vbo{}, ibo{};
	//GLuint fbo;
	std::map<unsigned int, BufferObject> buffer_handles;
	int fb_height;
	int fb_widht;
	unsigned int* frame_buffer;
	float* depth_buffer;
};


