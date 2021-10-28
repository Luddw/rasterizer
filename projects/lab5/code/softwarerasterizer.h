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
#include "rasterdata.h"



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


// struct Pixel
// {
// 	unsigned char r = 0;
// 	unsigned char g = 0;
// 	unsigned char b = 0;
// 	unsigned char a = 255;
// 	Pixel(unsigned char red, unsigned char green, unsigned char blu, unsigned char alf) : r(red), g(green), b(blu), a(alf) {};
// 	Pixel() {};

// };


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
	vec4 pos;
	vec3 uv;
	vec3 normal;
	vec3 color;
	VertexOut Interpolate(const VertexOut& target, float alpha) const
	{
		return {
				pos.interpolate(target.pos, alpha),
				uv.interpolate(target.uv, alpha),
				normal.interpolate(target.normal, alpha),
				color.interpolate(target.color, alpha)
		};

	}
};

struct Triangle
{
	Vertex v0;
	Vertex v1;
	Vertex v2;
	bool culled;
};

struct ModelMesh
{
	std::vector<Triangle> faces;
	std::vector<unsigned int> indexBuffer;
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
	const unsigned int AddModelBuff(std::vector<Model> &vbuff, std::vector<unsigned int> &ibuff);
	void SetupFrameBuffer(int width, int height);
	Pixel * GetFramebuffer();
	const int GetFramebufferSize();
	void PlacePixel(unsigned int x, unsigned int y, Pixel pix);
	void PutPixel(unsigned int x, unsigned int y, Pixel pix);
	void SetVertextShader(std::function<VertexOut(Vertex)> vertex_lambda);
	void SetFragmentShader(std::function<Pixel(VertexOut&, Texture&)> frag_lambda);
	int GetHeight();
	int GetWidth();
	void Draw(unsigned int handle);
	void RasterizeTriangle(vec3 v0, vec3 v1, vec3 v2, Pixel colour);
	void BarRasterizeTriangle(VertexOut* points, Pixel colour);
	void SetModelViewProjectionMatrix(const mat4 &mvp);
    void DrawLine(vec3 p1, vec3 p2);
	void SaveFB();
	bool LoadOBJModel(std::string filename);
	void UpdateQuadTex(GLuint handle);
	mat4 GetMVP();
	void ClearFB();
	void WireFrame(vec3 v0, vec3 v1, vec3 v2);
	void TriangleRaster(const VertexOut& v0, const VertexOut& v1, const VertexOut& v2);
	void NoCullBarRasterizeTriangle(vec3* pts, Pixel colour);
	vec4& ToScreenSpace(vec4& vec);
	bool Cull(vec4 v0, vec4 v1, vec4 v2) const;
	void SetTexture(Texture tex);
	VertexOut ApplyWeights(VertexOut v0, VertexOut v1, VertexOut v2, vec3 weights);
	void AddCube(float size);
	bool OBJLoad(const char* filename);
private:
	void FlatTopTriangle(const VertexOut& v0, const VertexOut& v1, const VertexOut& v2);
	void FlatBottomTriangle(const VertexOut& v0, const VertexOut& v1, const VertexOut& v2);


private:

	std::map<unsigned int, BufferObject> buffer_handles;
	int fb_height;
	int fb_width;
	Pixel * frame_buffer;
	float* depth_buffer;
	std::function<VertexOut(Vertex)> vertex_shader; 
	std::function<Pixel(VertexOut&, Texture&)> frag_shader;
	mat4 model_view_proj;
	mat4 viewMat;
	mat4 projMat;

	std::map<unsigned int, Model> model_handles;
	float width_offset;
	float height_offset;
	Texture texture;
	void InitResources();
};


