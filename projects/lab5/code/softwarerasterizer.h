#pragma once
#include <GL/glew.h>
#include <map>
#include <iterator>
#include "matlib.h"
#include <vector>
#include "meshresource.h"
#include <utility>
#include <functional>
#include "texture.h"
#include "modelraster.h"
#include "shaderresource.h"


struct BufferObject
{
	
	std::vector<Vertex> v_buffer;
	std::vector<unsigned int> i_buffer;

	BufferObject() : v_buffer(), i_buffer() {};
	BufferObject(std::vector<Vertex> v_buff, std::vector<unsigned int> i_buff)
	{
		this->v_buffer = v_buff;
		this->i_buffer = i_buff;
	};
};		


struct Pixel
{
	unsigned char r = 0;
	unsigned char g = 0;
	unsigned char b = 0;
	unsigned char a = 255;
	Pixel(unsigned char red, unsigned char green, unsigned char blu, unsigned char alf) : r(red), g(green), b(blu), a(alf) {};
	Pixel() {};

};



struct Point
{
	float xpos;
	float ypos;
	float zpos;
	
	Point operator -(const Point &p) const
	{
		// xpos = xpos - p.xpos;
		// ypos = ypos - p.ypos;
		return Point{xpos - p.xpos, ypos - p.ypos};
		
	};
	Point operator +(const Point &p) const
	{

		return Point{p.xpos + xpos, p.ypos + ypos};

	};
	Point operator *(const float &c)
	{
		xpos = xpos * c;
		ypos = ypos * c;
		return Point{xpos, ypos};

	};

	Point& operator =(const Point &p)
	{
		xpos = p.xpos;
		ypos = p.ypos;
		return *this;
	};
};

struct Line
{
	int dx, dy;
	Point start;
	Point end;
	Line(Point p1, Point p2) : start(p1), end(p2) {
		dx = std::abs(p2.xpos - p1.xpos);
		dy = std::abs(p2.ypos - p1.ypos); 
	};
};



struct VertexOut
{
	vec3 pos;
	vec3 uv;
};


class Renderer
{
public:
	Renderer();
	Renderer(const int width, const int height);
	~Renderer();
	void AddVertexBuffer(Vertex* buffer);
	void AddIndexBuffer(unsigned int* buffer);
	const unsigned int AddBuffer(std::vector<Vertex> &vbuff, std::vector<unsigned int> &ibuff, unsigned int faces);
	void SetupFrameBuffer(int width, int height);
	Pixel * GetFramebuffer();
	const int GetFramebufferSize();
	void PlacePixel(unsigned int x, unsigned int y, Pixel pix);
	void SetVertextShader(std::function<void(Vertex*)> vertex_lambda);
	void SetFragmentShader(std::function<Pixel(Vertex)> frag_lambda);
	int GetHeight();
	int GetWidth();
	void Draw(unsigned int handle);
	void RasterizeTriangle(vec3 v1, vec3 v2, vec3 v3, Pixel colour);
	void BarRasterizeTriangle(vec3* points, Pixel colour);
	void SetModelViewProjectionMatrix(const mat4 &mvp);
    void DrawLine(vec3 p1, vec3 p2);
	void SetTexture(const Texture &tex);
	void SaveFB();
	bool LoadOBJModel(std::string filename);
	void UpdateQuadTex(GLuint handle);
	mat4 GetMVP();
	void ClearFB();
	
private:
	std::map<unsigned int, BufferObject> buffer_handles;
	int fb_height;
	int fb_width;
	Pixel * frame_buffer;
	float* depth_buffer;
	std::function<void(Vertex*)> vertex_shader; 
	std::function<Pixel(Vertex)> frag_shader;
	mat4 model_view_proj;
	mat4 viewMat;
	mat4 projMat;


	/*Texture fb_tex;
	ShaderResource fb_shader;
	MeshResource fb_mesh;*/
	void InitResources();
};


