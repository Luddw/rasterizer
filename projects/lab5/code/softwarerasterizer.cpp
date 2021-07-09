#include "softwarerasterizer.h"
#include <functional>
#include <cmath>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "objloader.h"

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

void Renderer::DrawModel(MeshResource mesh)
{

	
	// for (size_t i = 0; i < model.nfaces(); i++)
	// {
	// 	std::vector<int> face 
	// 	Point coords[3];
	// 	for (size_t j = 0; i < j; j++)
	// 	{
	// 		vec3 world_coord = model.vert(face)
	// 	}
		
	// }
	
}

void Renderer::Draw(unsigned int handle)
{
	
	auto object = buffer_handles[handle];
	
	printf("objects to draw: %ld \n", buffer_handles.size());
	printf("v size: %ld | i size: %ld \n", object.v_buffer.size(), object.i_buffer.size());
		
	for (size_t i = 0; i < object.i_buffer.size(); i+=3)
	{
		auto p1 = object.v_buffer[object.i_buffer[i]].pos;
		auto p2 = object.v_buffer[object.i_buffer[i+1]].pos;
		auto p3 = object.v_buffer[object.i_buffer[i+2]].pos;
		Point point1 = {p1.x, p1.y, 0};
		Point point2 = {p2.x, p2.y, 0};
		Point point3 = {p3.x, p3.y, 0};
		
		printf("[%ld], x: %f, y: %f \n", i, point1.xpos, point1.ypos);
		RasterizeTriangle(point1, point2, point3, Pixel{40,50,100, 254});

	}


	

}

void Renderer::AddVertexBuffer(Vertex * buffer) 
{
	
}

void Renderer::AddIndexBuffer(unsigned int * buffer)
{
	
}

const unsigned int Renderer::AddBuffer(std::vector<Vertex> vbuff, std::vector<unsigned int> ibuff, unsigned int faces)
{
	unsigned int t_handle = 1;
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

void Renderer::DrawLine(Point p1, Point p2 , Pixel colour)
{
	bool isSteep = false;
	int temp_x1, temp_y1, temp_x2, temp_y2;
	temp_x1 = (p1.xpos * 0.5f + 0.5f) * fb_width;
	temp_x2 = (p2.xpos * 0.5f + 0.5f) * fb_width;
	temp_y1 = (p1.ypos * 0.5f + 0.5f) * fb_height;
	temp_y2 = (p2.ypos * 0.5f + 0.5f) * fb_height;
	
	p2.xpos = temp_x2;
	p2.ypos = temp_y2;
	p1.xpos = temp_x1;
	p1.ypos = temp_y1;
	if (std::abs(p1.xpos - p2.xpos) < std::abs(p1.ypos - p2.ypos))
	{
		std::swap(p1.xpos, p1.ypos);
		std::swap(p2.xpos, p2.ypos);
		isSteep = true;
	}

	if (p1.xpos > p2.xpos) // switch to left-to-right
	{
		std::swap(p1.xpos, p2.xpos);
		std::swap(p1.ypos, p2.ypos);
	}
	int dx = p2.xpos - p1.xpos;
	int dy = p2.ypos - p1.ypos;
	int dError = std::abs(dy) * 2;
	int error = 0;
	int y = p1.ypos;
	


	for (size_t x = temp_x1; x <= temp_x2; x++)
	{
		
		if (isSteep)
			PlacePixel(y, x, colour);
		else
			PlacePixel(x, y, colour);

		error += dError;
		if (error > dx)
		{
			y += (temp_y2 > temp_y1 ? 1 : -1);
			error -= dx * 2;
		}	
	}
	
	
	

}

void Renderer::PlaceTriangle(Point p1, Point p2, Point p3)
{
	/*DrawLine(p1.xpos, p1.ypos, p2.xpos, p2.ypos);
	DrawLine(p2.xpos, p2.ypos, p3.xpos, p3.ypos);
	DrawLine(p3.xpos, p3.ypos, p1.xpos, p1.ypos);*/
}

void Renderer::RasterizeTriangle(Point p1, Point p2, Point p3, Pixel colour)
{
	printf("rasterize....\n");
	// sort each vertex by Y-pos, if-ladder
	if (p1.ypos > p2.ypos)
		std::swap(p1, p2);
	if (p1.ypos > p3.ypos)
		std::swap(p1, p3);
	if (p2.ypos > p3.ypos)
		std::swap(p2, p3);
	printf("rasterize....\n");

	float height = p3.ypos - p1.ypos; //totalt "height" of the triangle to raster
	printf("rasterize....\n");


	for (int y = p1.ypos; y <= p2.ypos; y++)
	{
		int segmentHeight = p2.ypos - p1.ypos + 1; // first segment of triangle
		float a = (float)(y - p1.ypos) / height;
		float b = (float)(y - p1.ypos) / segmentHeight;
		Point A = p1 + (p3 - p1) * a;
		Point B = p1 + (p2 - p1) * b;

		PlacePixel(A.xpos, y, Pixel{254, 0,0,254});
		PlacePixel(B.xpos, y, Pixel{0, 254,0,254});
		if (A.xpos > B.xpos)
			std::swap(A, B);

		for (int ii = A.xpos; ii <= B.xpos; ii++) {
			PlacePixel(ii, y, colour);
		}

	}

	printf("rasterize....\n");


	for (int y = p2.ypos; y <= p3.ypos; y++)
	{
		int segmentHeight = p3.ypos - p2.ypos + 1; // second segment
		float a = (float)(y - p1.ypos) / height;
		float b = (float)(y - p2.ypos) / segmentHeight;
		Point A = p1 + (p3 - p1) * a;
		Point B = p2 + (p3 - p2) * b;
		if (A.xpos > B.xpos)
			std::swap(A, B);

		
		for (int ii = A.xpos; ii <= B.xpos; ii++) {
			PlacePixel(ii, y, colour);
		}
		
		

	}
	

	// DrawLine(p1, p2, Pixel{0,254,0,254});
	// DrawLine(p2, p3, Pixel{0,254,0,254});
	// DrawLine(p3, p1, Pixel{254,0,0,254});


	printf("triangle: p1:%f %f p2:%f %f p3:%f %f colr:%d %d %d \n",p1.xpos,p1.ypos,p2.xpos,p2.ypos,p3.xpos,p3.ypos,colour.r,colour.g,colour.b);
	

}

bool Renderer::LoadOBJModel(std::string filename)
{

	std::vector<Vertex> outVerts;
	std::vector<unsigned int> outIndices;
	Loader loader;
	bool loadout = loader.LoadFile(filename);
	if (loadout != true)
		return false;

	outIndices = loader.LoadedIndices;
	auto v = loader.LoadedMeshes;
	for (size_t i = 0; i < loader.LoadedVertices.size(); i++)
	{
		outVerts.emplace_back(loader.LoadedVertices[i]);
	}
	
	printf("vsize: %ld isize: %ld \n", outVerts.size(), outIndices.size());
	AddBuffer(outVerts, outIndices, 0);

	return true;
}
