#include "softwarerasterizer.h"




Renderer::Renderer() : fb_widht(0), fb_height(0), frame_buffer(nullptr), depth_buffer(nullptr)
{
}

Renderer::Renderer(const int widht, const int height)
{
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

void Renderer::SetupFrameBuffer(int widht, int height)
{
	frame_buffer = new unsigned[widht * height];
	depth_buffer = new float[widht * height];

}

unsigned int* Renderer::GetFramebuffer()
{
	return frame_buffer;
}

const int Renderer::GetFramebufferSize()
{
	return fb_widht*fb_height;
}
