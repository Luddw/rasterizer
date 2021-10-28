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


static float staticRotation = 0;
Renderer::Renderer() : fb_width(0), fb_height(0), frame_buffer(nullptr), depth_buffer(nullptr)
{

}

Renderer::Renderer(const int width, const int height)
{
	fb_width = width;
	fb_height = height;
	//frame_buffer = new Pixel[width * height];
	//depth_buffer = new float[width * height];
	height_offset = height / 2.0f;    
	width_offset = width / 2.0f;    
	SetupFrameBuffer(width, height);
	projMat = perspectiveprojection(pi/2.0f, 4.0f/3.0f, 0.1f, 100.0f);
	viewMat = lookat(vec3(0,0,1.0f), vec3(0,0,-1), vec3(0,1,0));
    

	
	Texture meshTex("./resources/error.png");
	texture = meshTex;  



	SetVertextShader([&](Vertex inVert) -> VertexOut {
		vec4 t_pos(inVert.pos.x, inVert.pos.y, inVert.pos.z, 1.0);
		

		mat4 translation(	1,0,1,0,
							0,1,0,0,
							0,0,1,0,
							0,0,-1,1

		);
		// mat4 scale(

		// 					vec4(0.25,0,0,0),
		// 					vec4(0,0.25,0,0),
		// 					vec4(0,0,0.25,0),
		// 					vec4(0,0,0,1)
		// );
		mat4 scale(

							vec4(0.5,0,0,0),
							vec4(0,0.5,0,0),
							vec4(0,0,0.5,0),
							vec4(0,0,0,1)
		);
		mat4 roty = rotationy(staticRotation);
		mat4 rotx = rotationx(staticRotation);
		
		mat4 rot = roty * rotx;
		
		mat4 model = scale * rot * translation;

		mat4 view = lookat(vec3(0,0,1), vec3(0,0,-1), vec3(0,1,0));
		mat4 proj = perspectiveprojection(pi/2.0f, 4.0f/3.0f, 0.1f, 100.0f);

		t_pos = model * t_pos;
		t_pos = transform(t_pos, view);
		t_pos = transform(t_pos, proj);
		VertexOut out{t_pos, inVert.uv, inVert.normal, vec3(0,0,0)};
		return out;
	});

	SetFragmentShader([&](VertexOut& inVert, Texture& tex) -> Pixel {
		Pixel texcol;
		vec3 lightcolor(254,254,254);
		vec3 light_pos(1,1,0);
		vec3 cam_pos(0,0,1);
		texcol = tex.GetColor(inVert.uv);
		vec3 texture_color(texcol.r,texcol.g,texcol.b);
		float intensity = 0.9f;

		vec3 ambient = lightcolor * intensity;
		vec3 frag_norm = normalize(inVert.normal);
		vec3 light_direction = normalize(light_pos - inVert.pos);

		float diffuse_intensity = fmax(dot(frag_norm, light_direction), 0.0f);

		vec3 diffuse = lightcolor * diffuse_intensity;
		
		float specular_intensity = 0.5f;
		vec3 view_direction = normalize(cam_pos - inVert.pos);
		vec3 halfway_direction = normalize(light_direction + view_direction);
		float specular_value = pow(fmax(dot(frag_norm, halfway_direction), 0.0f), 32);

		vec3 specular = lightcolor * specular_intensity * specular_value;

		vec3 finalcolor(ambient + diffuse + specular);
		finalcolor = mul(finalcolor, texture_color);
		//outColor.r = 244;
		Pixel out(finalcolor.x * 255, finalcolor.x * 255,finalcolor.x * 255, 255);
		//Pixel outColor(inVert.uv.x * 255, inVert.uv.y * 255, 0, 255);
		//out = Pixel(inVert.uv.x * 255, inVert.uv.y * 255, 0 * 255, 255);
		return out;
		//return tex.GetColor(inVert.uv);
	});
}

Renderer::~Renderer()
{
	//delete depth_buffer;
}


void Renderer::Draw(unsigned int handle)
{
	const BufferObject object = buffer_handles[handle];
	int randomcolor;
	size_t vbuffsize = object.v_buffer.size();
	VertexOut* outVert = new VertexOut[vbuffsize];
	
	for (size_t i = 0; i < object.i_buffer.size(); i++)
	{
		outVert[i] = vertex_shader(object.v_buffer[i]);

	}
	for (size_t i = 0; i < object.i_buffer.size(); i+=3)
	{
		if (Cull(outVert[i].pos, outVert[i+1].pos, outVert[i+2].pos))
			continue;

		randomcolor = (i/3) % 2;

		ToScreenSpace(outVert[i].pos);
		ToScreenSpace(outVert[i+1].pos);
		ToScreenSpace(outVert[i+2].pos);

		TriangleRaster(outVert[i], outVert[i+1], outVert[i+2], Pixel{255 * randomcolor,0,255 * randomcolor,255});
	}
	
	staticRotation += 0.01f;
	delete outVert;
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
	for (size_t i = 0; i < width * height; i++)
		depth_buffer[i] = -std::numeric_limits<float>::max();

	
	
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

void Renderer::SetVertextShader(std::function<VertexOut(Vertex)> vertex_lambda)
{
	this->vertex_shader = vertex_lambda;
}

void Renderer::SetFragmentShader(std::function<Pixel(VertexOut&, Texture&)> frag_lambda)
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
	stbi_write_png("capture.png", fb_width, fb_height, 4, frame_buffer, fb_width*4);
	printf("writing image: %s \n", "capture.png");
}



//bresenham line function 
void Renderer::DrawLine(vec3 p1, vec3 p2)
{
    bool isSteep = false;

    
    if (std::abs(p1.x - p2.x) < std::abs(p1.y - p2.y))
    {
        std::swap(p1.x, p1.y);
        std::swap(p2.x, p2.y);
        isSteep = true;
    }

    if (p1.x > p2.x) // switch to left-to-right
    {
        std::swap(p1.x, p2.x);
        std::swap(p1.y, p2.y);
    }
    int dx = p2.x - p1.x;
    int dy = p2.y - p1.y;
    int dError = std::abs(dy) * 2;
    int error = 0;
    int y = p1.y;
    

    for (size_t x = p1.x; x <= p2.x; x++)
    {
        
        if (isSteep)
            PlacePixel(y, x, Pixel(0,0,0,255));
        else
            PlacePixel(x, y, Pixel(0,255,0,255));

        error += dError;
        if (error > dx)
        {
            y += (p2.y > p1.y ? 1 : -1);
            error -= dx * 2;
        }    
    }
    
    
    

}


//scanline raster
void Renderer::RasterizeTriangle(vec3 v0, vec3 v1, vec3 v2, Pixel colour)
{
	if (v0.y == v1.y && v0.y == v2.y)
		return;
	
    if (v0.y > v1.y)
        std::swap(v0, v1);
    if (v0.y > v2.y)
        std::swap(v0, v2);
    if (v1.y > v2.y)
        std::swap(v1, v2);

    float height = v2.y - v0.y; //total "height" of the triangle to raster

    // scanline rasterization, both segments
    for (size_t i = 0; i < height; i++)
    {        
        bool isSegmentTwo = i > v1.y - v0.y || v1.y == v0.y;
        int heightSegment = isSegmentTwo ? v2.y - v1.y : v1.y - v0.y;

        float a = (float)i / height;
        float b = (float)(i - (isSegmentTwo ? v1.y - v0.y : 0))/heightSegment;
        vec3 A = v0 + vec3(v2 - v0) * a;
        vec3 B = isSegmentTwo ? v1 + vec3(v2 - v1) * b : v0 + vec3(v1 - v0) * b;
        if (A.x > B.x)
            std::swap(A, B);

        for (size_t ii = A.x; ii <= B.x; ii++)
        {
            float k = B.x == A.x ? 1.0f : (float)(ii - A.x) / (float)(B.x - A.x);
            vec3 P = vec3(A) + vec3(B - A)*k;
            int index = P.x + P.y * fb_width;
            if (depth_buffer[index] < P.z)
            {
                depth_buffer[index] = P.z;
                PlacePixel(P.x, P.y, Pixel{254 * P.z, 254 * P.z, 254 * P.z, 254});
            }    
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
		outVerts.emplace_back(loader.LoadedVertices[i]);

	}
	
	AddBuffer(outVerts, outIndices, 0);

	
	return true;
}



mat4 Renderer::GetMVP()
{
	return mat4(vec4(1,0,0,0),
				vec4(0,1,0,0),
				vec4(0,0,1,0),
				vec4(0,0,0,1)) * viewMat * projMat ;
}

void Renderer::UpdateQuadTex(GLuint handle)
{

	glBindTexture(GL_TEXTURE_2D, handle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexSubImage2D(GL_TEXTURE_2D,0,0,0,fb_width,fb_height,GL_RGBA,GL_UNSIGNED_BYTE, frame_buffer);
	//glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,fb_width,fb_height,0,GL_RGBA,GL_UNSIGNED_BYTE, frame_buffer);
	glBindTexture(GL_TEXTURE_2D, 0);
}
void Renderer::ClearFB()
{
	for (size_t i = 0; i < fb_height*fb_width; i++)
	{
		frame_buffer[i].r = 200;
		frame_buffer[i].g = 200; 
		frame_buffer[i].b = 254; 
		frame_buffer[i].a = 254; 

	}
	for (size_t i = 0; i < fb_width * fb_height; i++)
	{
		depth_buffer[i] = -std::numeric_limits<float>::infinity();
	}


	
}
void Renderer::WireFrame(vec3 v0, vec3 v1, vec3 v2)
{
	DrawLine(v0, v1);
	DrawLine(v1, v2);
	DrawLine(v2, v0);
}



void Renderer::TriangleRaster(const VertexOut& v0, const VertexOut& v1, const VertexOut& v2, Pixel color)
{
	
	const VertexOut* p_v0 = &v0;
	const VertexOut* p_v1 = &v1;
	const VertexOut* p_v2 = &v2;
	
	// sort height of vertices
	if (p_v1->pos.y < p_v0->pos.y)
		std::swap(p_v0, p_v1);
	if (p_v2->pos.y < p_v1->pos.y)
		std::swap(p_v1, p_v2);
	if (p_v1->pos.y < p_v0->pos.y)
		std::swap(p_v0, p_v1);
	
	// identify the type of triangle, top-  bot- right- left- major

	if (p_v0->pos.y == p_v1->pos.y)	// top flat
	{
		// sort vertices by x-coord
		if (p_v1->pos.x < p_v0->pos.x)
			std::swap(p_v0, p_v1);

		FlatTopTriangle(*p_v0, *p_v1, *p_v2, color);
	}
	else if (p_v1->pos.y == p_v2->pos.y)	// bottom flat
	{
		// sort vertices by x-coord
		if (p_v2->pos.x < p_v1->pos.x)
			std::swap(p_v1, p_v2);
		
		FlatBottomTriangle(*p_v0, *p_v1, *p_v2, color);
	}
	else // any triangle
	{
		// find top bottom split vertex
		float k_split = (p_v1->pos.y - p_v0->pos.y) / (p_v2->pos.y - p_v0->pos.y);
		VertexOut vk = p_v0->Interpolate(*p_v2, k_split);

		if (p_v1->pos.x < vk.pos.x) // right major
		{
			FlatBottomTriangle(*p_v0, *p_v1, vk, color);
			FlatTopTriangle(*p_v1, vk, *p_v2, color);
		}
		else	// left major
		{
			FlatBottomTriangle(*p_v0, vk, *p_v1, color);
			FlatTopTriangle(vk, *p_v1, *p_v2, color);
		}
		
	}
	
}

void Renderer::FlatTopTriangle(const VertexOut& v0, const VertexOut& v1, const VertexOut& v2, Pixel color)
{


	// calc line slopes in screen-space
	float m0 = (v2.pos.x - v0.pos.x) / (v2.pos.y - v0.pos.y);
	float m1 = (v2.pos.x - v1.pos.x) / (v2.pos.y - v1.pos.y);

	// start and end for scanlines
	int scan_start = (int)ceil(v0.pos.y - 0.5f);
	int scan_end = (int)ceil(v2.pos.y - 0.5f);

	VertexOut P;
	for (P.pos.y = scan_start; P.pos.y < scan_end; P.pos.y++)
	{
		// scanline start X
		float px0 = m0 * ((P.pos.y) + 0.5f - v0.pos.y) + v0.pos.x;
		float px1 = m1 * ((P.pos.y) + 0.5f - v1.pos.y) + v1.pos.x;

		// start, end pixels
		int pix_start = (int)ceil(px0 - 0.5f);
		int pix_end = (int)ceil(px1 - 0.5f);

		for (P.pos.x = pix_start; P.pos.x < pix_end; P.pos.x++)
		{
			//vec3 pts[3] = {v0.pos,v1.pos,v2.pos};
//			P.pos.z = 0;
			vec3 weights = barycentric(v0.pos, v1.pos, v2.pos, P.pos);


			// if (weights.x < 0 || weights.y < 0 || weights.z < 0) //if degenerate
			// 	continue;

			P = ApplyWeights(v0, v1, v2, weights);





			if (depth_buffer[int(P.pos.x + P.pos.y * fb_width)] < P.pos.z)
			{
				Pixel texcol = frag_shader(P, texture);
				depth_buffer[int(P.pos.x + P.pos.y * fb_width)] = P.pos.z;
				PlacePixel(P.pos.x, P.pos.y, texcol);
			}
			
		}
		
	}
	

}

void Renderer::FlatBottomTriangle(const VertexOut& v0, const VertexOut& v1, const VertexOut& v2, Pixel color)
{

	// calc line slopes in screen-space
	float m0 = (v1.pos.x - v0.pos.x) / (v1.pos.y - v0.pos.y);
	float m1 = (v2.pos.x - v0.pos.x) / (v2.pos.y - v0.pos.y);

	// start and end for scanlines
	int scan_start = (int)ceil(v0.pos.y  - 0.5f);
	int scan_end = (int)ceil(v2.pos.y - 0.5f);

	VertexOut P;
	for (P.pos.y = scan_start; P.pos.y < scan_end; P.pos.y++)
	{
		// scanline start X
		float px0 = m0 * ((P.pos.y) + 0.5f - v0.pos.y) + v0.pos.x;
		float px1 = m1 * ((P.pos.y) + 0.5f - v0.pos.y) + v0.pos.x;

		// start, end pixels
		int pix_start = (int)ceil(px0 - 0.5f);
		int pix_end = (int)ceil(px1 - 0.5f);

		for (P.pos.x = pix_start; P.pos.x < pix_end; P.pos.x++)
		{
			//vec3 pts[3] = {v0.pos,v1.pos,v2.pos};
//			P.pos.z = 0;

			vec3 weights = barycentric(v0.pos, v1.pos, v2.pos, P.pos);
			//if (weights.x < 0 || weights.y < 0 || weights.z < 0)
			//	continue;


			

			P = ApplyWeights(v0, v1, v2, weights);


			if (depth_buffer[int(P.pos.x + P.pos.y * fb_width)] < P.pos.z)
			{
				depth_buffer[int(P.pos.x + P.pos.y * fb_width)] = P.pos.z;
				Pixel texcol = frag_shader(P, texture);
				PlacePixel(P.pos.x, P.pos.y, texcol);
			}
			
			
		}
		
	}
	
}

VertexOut Renderer::ApplyWeights(VertexOut v0, VertexOut v1, VertexOut v2, vec3 weights)
{
	VertexOut out;
	out.pos = v0.pos * weights.x + v1.pos * weights.y + v2.pos * weights.z;
	out.uv = v0.uv * weights.x + v1.uv * weights.y + v2.uv * weights.z;
	out.normal = v0.normal * weights.x + v1.normal * weights.y + v2.normal * weights.z;
	out.color = v0.color * weights.x + v1.color * weights.y + v2.color * weights.z;
	return out;
}

vec4& Renderer::ToScreenSpace(vec4& vec)
{
	vec.x / vec.w;
	vec.y / vec.w;
	vec.z / vec.w;

	vec.x = (vec.x + 1.0f) * width_offset; 
	vec.y = (vec.y + 1.0f) * height_offset; 
	vec.z = (vec.y + 1.0f); 

	return vec;
}


bool Renderer::Cull(vec4 v0, vec4 v1, vec4 v2) const
{
	vec3 a(v0);
	vec3 b(v1);
	vec3 c(v2);
	
	return dot(cross((v1 - v0), (v2 - v0)), v0) > 0;
}



void Renderer::SetTexture(Texture tex)
{
	texture = tex;
}
