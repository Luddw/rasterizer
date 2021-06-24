#include "softwarerasterizer.h"
#include <functional>
#include <cmath>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


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

void Renderer::SetVertextShader(std::function<Vertex(Vertex)> vertex_lambda)
{
	this->vertex_shader = vertex_lambda;
}

void Renderer::SetFragmentShader(std::function<void(Vertex)> frag_lambda)
{
	this->frag_shader = frag_lambda;
}

int Renderer::GetHeight()
{
	return fb_height;
}

int Renderer::GetWidth()
{
	return fb_width;
}

void Renderer::SetModelViewProjectionMatrix(const Matrix4D &mvp)
{
	model_view_proj = mvp;
}

void Renderer::SaveFB()
{
	stbi_write_png("gamer.png", fb_width, fb_height, 4, frame_buffer, fb_width*4);
	printf("writing image: %s \n", "gamer.png");
}

void Renderer::SetTexture(const Texture &tex)
{
	this->tex = tex;
}

void Renderer::DrawLine(int x0, int y0, int x1, int y1)
{
	Pixel p(0xFF,0,0,0xFF);
	bool isSteep = false;
	if (std::abs(x0 - x1) < std::abs(y0 - y1))
	{
		std::swap(x0, y0);
		std::swap(x1, y1);
		isSteep = true;
	}

	if (x0 > x1) // switch to left-to-right
	{
		std::swap(x0, x1);
		std::swap(y0, y1);
	}
	int dx = x1 - x0;
	int dy = y1 - y0;
	int dError = std::abs(dy) * 2;
	int error = 0;
	int y = y0;
	
	for (size_t x = x0; x <= x1; x++)
	{
		if (isSteep)
			PlacePixel(y, x, p);
		else
			PlacePixel(x, y, p);

		error += dError;
		if (error > dx)
		{
			y += (y1 > y0 ? 1 : -1);
			error -= dx * 2;
		}	
		// float k = (x-x0) / (float)(x1-x0);
		// int y = y0 * (1.0 - k) + y1 * k;
		// if (isSteep)
		// 	PlacePixel(y, x, p);
		// else
		// 	PlacePixel(x, y, p);
	}
	
	
	

	// for (size_t x = x0; x < x1; x++)
	// {
	// 	float k = (x-x0) / (float)(x1-x0);
	// 	int y = y0 * (1.0 - k) + (y1 * k);
	// 	PlacePixel(x, y, p);	
	// }
	
}

void Renderer::PlaceTriangle(Point p1, Point p2, Point p3)
{
	DrawLine(p1.xpos, p1.ypos, p2.xpos, p2.ypos);
	DrawLine(p2.xpos, p2.ypos, p3.xpos, p3.ypos);
	DrawLine(p3.xpos, p3.ypos, p1.xpos, p1.ypos);
}