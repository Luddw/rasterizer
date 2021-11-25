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




struct VertexOut
{
	vec4 pos;
	vec3 uv;
	vec3 normal;
	vec3 frag_pos;
	VertexOut Interpolate(const VertexOut& target, float alpha) const
	{
		return {
				pos.interpolate(target.pos, alpha),
				uv.interpolate(target.uv, alpha),
				normal.interpolate(target.normal, alpha),
				frag_pos.interpolate(target.frag_pos, alpha)
		};

	}
};

struct VertexOBJref
{
	VertexOBJref(int v, int vt, int vn) : v(v), vt(vt), vn(vn) {}
	int v, vt, vn;
};


struct Line
{
	VertexOut p0;
	VertexOut p1;
	std::vector<vec3> plots;

};



class Renderer
{
public:
	Renderer();
	Renderer(const int width, const int height);
	~Renderer();
	void AddVertexBuffer(Vertex* buffer);
	void AddIndexBuffer(unsigned int* buffer);
	const unsigned int AddBuffer(std::vector<Vertex> &vbuff, std::vector<unsigned int> &ibuff);
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
	void RasterizeTriangle(VertexOut v0, VertexOut v1, VertexOut v2);
	void SetModelViewProjectionMatrix(const mat4 &mvp);
    Line DrawLine(vec3 p1, vec3 p2);
	void SaveFB();
	bool LoadOBJModel(std::string filename);
	void UpdateQuadTex(GLuint handle);
	mat4 GetMVP();
	void ClearFB();
	void Triangle(VertexOut v0, VertexOut v1, VertexOut v2);
	void TriangleRaster(const VertexOut& v0, const VertexOut& v1, const VertexOut& v2);
	void NoCullBarRasterizeTriangle(vec3* pts, Pixel colour);
	vec4& ToScreenSpace(vec4& vec);
	bool Cull(vec4 v0, vec4 v1, vec4 v2) const;
	void SetTexture(Texture tex);
	VertexOut ApplyWeights(VertexOut v0, VertexOut v1, VertexOut v2, vec3 weights);
	Line BresenLine(int x0, int y0, int x1, int y1);
	std::vector<Vertex> OBJLoader(const char* filepath);
	std::vector<Vertex> GetMesh();
private:
	void FlatTopTriangle(const VertexOut& v0, const VertexOut& v1, const VertexOut& v2);
	void FlatBottomTriangle(const VertexOut& v0, const VertexOut& v1, const VertexOut& v2);


private:

	//std::map<unsigned int, BufferObject> buffer_handles;
	int fb_height;
	int fb_width;
	Pixel * frame_buffer;
	float* depth_buffer;
	std::function<VertexOut(Vertex)> vertex_shader; 
	std::function<Pixel(VertexOut&, Texture&)> frag_shader;

	std::vector<Vertex> model;
	std::vector<BufferObject> buffer_handles;
	float width_offset;
	float height_offset;
	Texture texture;
};


