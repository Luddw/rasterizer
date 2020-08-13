#include "softwarerasterizer.h"
#include <functional>


Renderer::Renderer() : fb_width(0), fb_height(0), frame_buffer(nullptr), depth_buffer(nullptr)
{
}

Renderer::Renderer(const int width, const int height)
{
	fb_width = width;
	fb_height = height;
	SetupFrameBuffer(width, height);
}

Renderer::~Renderer()
{
}

void Renderer::AddVertexBuffer(Vertex * buffer) 
{
	
}

void Renderer::AddIndexBuffer(unsigned int * buffer)
{
	
}

const unsigned int Renderer::AddBuffer(std::vector<Vertex> vbuff, std::vector<unsigned int> ibuff)
{
	unsigned int t_handle = buffer_handles.size() + 1;
	buffer_handles.emplace(t_handle, BufferObject(vbuff, ibuff));
	return t_handle;
}

void Renderer::SetupFrameBuffer(int width, int height)
{
	frame_buffer = new Pixel[width * height];
	depth_buffer = new float[width * height];
}

Pixel * Renderer::GetFramebuffer()
{
	return frame_buffer;
}

const int Renderer::GetFramebufferSize()
{
	return fb_width*fb_height;
}

void Renderer::PlacePixel(unsigned int x, unsigned int y, Pixel pix)
{
	if (x < fb_width && y < fb_height)
		this->frame_buffer[x + (y * fb_width)] = pix;
}

void Renderer::SetVertextShader(std::function<Vertex(Vertex)> VertexLambda)
{
	this->VertexShader = VertexLambda;
}

void Renderer::SetFragmentShader(std::function<void(Vertex)> FragLambda)
{
	this->FragShader = FragLambda;
}

int Renderer::GetHeight()
{
	return fb_height;
}

int Renderer::GetWidth()
{
	return fb_width;
}

void Renderer::SetModelViewProjectionMatrix(Matrix4D mvp)
{
	model_view_proj = mvp;
}

void Renderer::SetTexture(Texture tex)
{
	this->tex = tex;
}
