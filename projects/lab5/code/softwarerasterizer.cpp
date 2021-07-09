#include "softwarerasterizer.h"
#include <functional>
#include <cmath>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <iostream>
#include <fstream>
#include <sstream>

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

void Renderer::Draw()
{

	
	for (auto const &object : buffer_handles)
	{
		printf("ibo size: %ld \n", object.second.i_buffer.size());
		for (auto const &index: object.second.i_buffer)
		{
			printf("%d ", index);
		}
		printf("\n");
		for (auto vert: object.second.v_buffer)
		{
			printf("vert:  x: %f y: %f \n", vert.pos.x, vert.pos.y);
		}
		
		for (size_t i = 0; i < object.second.i_buffer.size(); i+=3)
		{
			auto p1 = object.second.v_buffer[object.second.i_buffer[i]].pos;
			auto p2 = object.second.v_buffer[object.second.i_buffer[i+1]].pos;
			auto p3 = object.second.v_buffer[object.second.i_buffer[i+2]].pos;
			Point point1 = {p1.x, p1.y};
			Point point2 = {p2.x, p2.y};
			Point point3 = {p3.x, p3.y};
			
			printf("[%ld], x: %f, y:%f \n", i, point1.xpos, point1.ypos);
			RasterizeTriangle(point1, point2, point3, Pixel{40,50,100, 254});
		}
		
		printf("ending");
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
	// sort each vertex by Y-pos, if-ladder
	if (p1.ypos > p2.ypos)
		std::swap(p1, p2);
	if (p1.ypos > p3.ypos)
		std::swap(p1, p3);
	if (p2.ypos > p3.ypos)
		std::swap(p2, p3);

	int height = p3.ypos - p1.ypos; //totalt "height" of the triangle to raster


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
	
	
	// DrawLine(p1, p2, Pixel{0,254,0,254});
	// DrawLine(p2, p3, Pixel{0,254,0,254});
	// DrawLine(p3, p1, Pixel{254,0,0,254});


	printf("triangle: p1:%f %f p2:%f %f p3:%f %f colr:%d %d %d \n",p1.xpos,p1.ypos,p2.xpos,p2.ypos,p3.xpos,p3.ypos,colour.r,colour.g,colour.b);
	

}

void Renderer::LoadOBJModel(const char* filename)
{
	std::vector<Vertex> outVerts;
	std::vector<unsigned int> outIndices;
	std::vector<GLuint> vertexIndices, uvIndices, normIndices;
	std::vector<vec3> t_verts;
	std::vector<vec3> t_uvs;
	std::vector<vec4> t_norms;

	std::ifstream stream(filename);
	std::string line;

	enum type
	{
		v, vt, vn, f, none
	};

	printf("loading file: %s \n", filename);
	while (getline(stream, line))
	{
		std::string tmp;
		std::stringstream ss(line);
		std::vector<std::string> tokens;
		while (getline(ss, tmp, ' '))
		{
			tokens.emplace_back(tmp);
		}
		if (tokens.empty())
			continue;

		type t = none;
		if (tokens[0] == "v")
			t = v;
		else if (tokens[0] == "vt")
			t = vt;
		else if (tokens[0] == "vn")
			t = vn;
		else if (tokens[0] == "f")
			t = f;
		else if (tokens[0] == "#")
			continue;

		//std::cout << tokens[0] << std::endl;

		switch (t)
		{
		case v:
		{
			vec3 vert;
			for (size_t i = 1; i < 4; i++)
			{
				scanf(tokens[i].c_str(), "%f", &vert[i - 1]);
			}

			t_verts.emplace_back(vert);
			break;
		}
		case vt:
		{
			vec3 uv(0,0,0);
			for (size_t i = 1; i < 3; i++)
			{
				scanf(tokens[i].c_str(), "%f", &uv[i - 1]);

			}


			//t_uvs.emplace_back(uv);
			break;
		}
		case vn:
		{
			vec4 norm;
			for (size_t i = 1; i < 4; i++)
			{
				scanf(tokens[i].c_str(), "%f", &norm[i - 1]);

			}

			t_norms.emplace_back(norm);
			break;
		}
		case f:
		{
			unsigned int vert, uvs, norms;


			if (tokens.size() == 4) //triangle
			{
				for (size_t i = 1; i < 4; i++)
				{
					//scanf(tokens[i].c_str(), "%d/%d/%d", &verts,  &uvs, &norms);
					scanf(tokens[i].c_str(), "%d/%d/%d", &vert, &uvs, &norms);


					vertexIndices.emplace_back(vert);
					uvIndices.emplace_back(uvs);
					normIndices.emplace_back(norms);
				}


			}
			else if (tokens.size() == 5) //quad
			{
				std::vector<GLuint> tempverts, tempuvs, tempnorms;
				for (size_t i = 1; i < 5; i++)
				{
					scanf(tokens[i].c_str(), "%d/%d/%d" , &vert, &uvs, &norms);


					tempverts.emplace_back(vert);
					tempuvs.emplace_back(uvs);
					tempnorms.emplace_back(norms);
				}
				vertexIndices.emplace_back(tempverts[0]);
				vertexIndices.emplace_back(tempverts[1]);
				vertexIndices.emplace_back(tempverts[3]);
				vertexIndices.emplace_back(tempverts[2]);
				vertexIndices.emplace_back(tempverts[3]);
				vertexIndices.emplace_back(tempverts[1]);

				uvIndices.emplace_back(tempuvs[0]);
				uvIndices.emplace_back(tempuvs[1]);
				uvIndices.emplace_back(tempuvs[3]);
				uvIndices.emplace_back(tempuvs[2]);
				uvIndices.emplace_back(tempuvs[3]);
				uvIndices.emplace_back(tempuvs[1]);

				normIndices.emplace_back(tempnorms[0]);
				normIndices.emplace_back(tempnorms[1]);
				normIndices.emplace_back(tempnorms[3]);
				normIndices.emplace_back(tempnorms[2]);
				normIndices.emplace_back(tempnorms[3]);
				normIndices.emplace_back(tempnorms[1]);

			}


			break;
		}
		default:
			break;
		}

	}
	std::vector<vec4> buf;
	// vertexss.clear();
	// vert.clear();
	// indices.clear();

	for (size_t i = 0; i < vertexIndices.size(); i++)
	{
		unsigned int vertIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normIndex = normIndices[i];

	
		vec3 vertex = t_verts[vertIndex - 1];
		vec3 uv = t_uvs[uvIndex - 1];
		vec4 norm = t_norms[normIndex - 1];

		outVerts.emplace_back(Vertex(vertex, uv, norm));		
		outIndices.emplace_back(i);
	}

	std::cout <<"pos: " << vertexIndices.size() << std::endl;
	std::cout <<"uvs: " << uvIndices.size() << std::endl;
	std::cout <<"norms: " << normIndices.size() << std::endl;

	AddBuffer(outVerts, outIndices, 0);


}
