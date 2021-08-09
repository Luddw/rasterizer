#include "softwarerasterizer.h"
#include <functional>
#include <cmath>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "objloader.h"
#include <algorithm>
#include <time.h>




Renderer::Renderer() : fb_width(0), fb_height(0), frame_buffer(nullptr), depth_buffer(nullptr)
{

}

Renderer::Renderer(const int width, const int height)
{
	fb_width = width;
	fb_height = height;
	SetupFrameBuffer(width, height);
	projMat = perspectiveprojection(1.5705f, width/height, 0.0f, 1.0f);
	viewMat = lookat(vec3(0,0,1.0f), vec3(0,0,0), vec3(0,1,0));
	model_view_proj = GetMVP();
}

Renderer::~Renderer()
{

}


void Renderer::Draw(unsigned int handle)
{
	
	const BufferObject object = buffer_handles[handle];
	SetVertextShader([&](Vertex v, void* data){
		VertexOut
	});
	for (size_t i = 0; i < object.i_buffer.size(); i+=3)
	{
		vec3 points[3];
		Vertex verts[3];

		points[0] = object.v_buffer[object.i_buffer[i]].pos;
		points[1] = object.v_buffer[object.i_buffer[i+1]].pos;
		points[2] = object.v_buffer[object.i_buffer[i+2]].pos;

		verts[0] = object.v_buffer[object.i_buffer[i]];
		verts[1] = object.v_buffer[object.i_buffer[i+1]];
		verts[2] = object.v_buffer[object.i_buffer[i+2]];

		vertex_shader(verts[0], verts);
		BarRasterizeTriangle(points, Pixel{rand() % 254, rand() % 254, rand()% 254, 254});
	}
	

	


	

}

void Renderer::AddVertexBuffer(Vertex * buffer) 
{
	
}

void Renderer::AddIndexBuffer(unsigned int * buffer)
{
	
}

const unsigned int Renderer::AddBuffer(std::vector<Vertex> &vbuff, std::vector<unsigned int> &ibuff, unsigned int faces)
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

void Renderer::SetVertextShader(std::function<void(Vertex, void*)> vertex_lambda)
{
	this->vertex_shader = vertex_lambda;
}

void Renderer::SetFragmentShader(std::function<void(Vertex, void*)> frag_lambda)
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

void Renderer::SetModelViewProjectionMatrix(const mat4 &mvp)
{
	model_view_proj = mvp;
}

//save framebuffer as png in running directory
void Renderer::SaveFB()
{
	stbi_flip_vertically_on_write(true);
	stbi_write_png("gamer.png", fb_width, fb_height, 4, frame_buffer, fb_width*4);
	printf("writing image: %s \n", "gamer.png");
}



//bresenham line function 
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


//scanline raster
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

	float height = p3.ypos - p1.ypos; //totalt "height" of the triangle to raster


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
}

bool Renderer::LoadOBJModel(std::string filename)
{

	std::vector<Vertex> outVerts;
	std::vector<unsigned int> outIndices;
	Loader loader;
	bool loadout = loader.LoadFile(filename);
	if (loadout != true)
		return false;

	outIndices = loader.LoadedMeshes[0].Indices;
	
	for (size_t i = 0; i < loader.LoadedVertices.size(); i++)
	{
		loader.LoadedVertices[i].pos *= 0.5f;
		loader.LoadedVertices[i].pos.z += 1.f;

		outVerts.emplace_back(loader.LoadedVertices[i]);
	}
	
	AddBuffer(outVerts, outIndices, 0);

	
	return true;
}
//bounding box barycoord raster
void Renderer::BarRasterizeTriangle(vec3* points, Pixel colour)
{
	//convert from opengl-space to fb-space , half-pixel offset

	for (size_t i = 0; i < 3; i++)
	{
		
	}
	
	for (size_t ii = 0; ii < 3; ii++)
	{
		
		int temp_x, temp_y;
		temp_x = (points[ii].x + 0.5f) * fb_width / 2.0f;
		temp_y = (points[ii].y + 0.5f) * fb_height / 2.0f;
		points[ii].x = temp_x;
		points[ii].y = temp_y;
	}

	//setup Bounding box from vertices

	vec3 boundingboxMIN(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
	vec3 boundingboxMAX(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
	vec3 clamped(fb_width -1, fb_height - 1);
	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 2; j++)
		{
			// TODO: static cast, ugly --> change matlib in future for different vec data-types i.e float, int
			boundingboxMIN[j] = std::max(0, static_cast<int>(std::min(boundingboxMIN[j], points[i][j])));
			boundingboxMAX[j] = std::min(static_cast<int>(clamped[j]), static_cast<int>(std::max(boundingboxMAX[j], points[i][j])));
		}
	}
	
	vec3 P;
	//traverse the bounding box, place pixels inside of bctriangles
	// "for each pixel ..."	
	for (P.x = boundingboxMIN.x; P.x <= boundingboxMAX.x; P.x++)
	{
		for (P.y = boundingboxMIN.y; P.y <= boundingboxMAX.y; P.y++)
		{
			vec3 screenBarycentric = barycentric(points, P);

			if (screenBarycentric.x < 0 || screenBarycentric.y < 0 || screenBarycentric.z < 0) // if clip
				continue;

			P.z = 0;
			for (size_t i=0; i < 3; i++)	//multiply each points Z-value with the bc-coord
			{
				P.z += points[i].z * screenBarycentric[i];
			}

			if (depth_buffer[int(P.x + P.y * fb_width)] < P.z) // check depth, discard otherwise
			{
				depth_buffer[int(P.x + P.y * fb_width)] =  P.z;
				PlacePixel(P.x, P.y, colour);
			}
			
		}
	}
	
}


mat4 Renderer::GetMVP()
{
	return mat4(vec4(1,0,0,0),
				vec4(0,1,0,0),
				vec4(0,0,1,0),
				vec4(0,0,0,1)) * viewMat * projMat ;
}


