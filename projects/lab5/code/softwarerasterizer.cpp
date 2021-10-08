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


static float gamer = 0;
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
    
	for (size_t i = 0; i < 12; i+=3)
	{
		colors[i]	= Pixel{254-i*4, 254, 254-i*4, 255};
		colors[i+1] = Pixel{0+i*2, 254-i*2, 254, 255};
		colors[i+2] = Pixel{0+i*3, i*4, i*5, 255};
	}



	SetVertextShader([&](Vertex& inVert) {
		vec4 t_pos(inVert.pos.x, inVert.pos.y, inVert.pos.z, 1.0);
		
		// mat4 translation(	vec4(1,0,1,0),
		// 					vec4(0,1,0,0),
		// 					vec4(0,0,1,0),
		// 					vec4(1,1,0,1)

		// );
		mat4 translation(	1,0,1,0,
							0,1,0,0,
							0,0,1,0,
							0,0,-3,1

		);
		mat4 scale(

							vec4(0.25,0,0,0),
							vec4(0,0.25,0,0),
							vec4(0,0,0.25,0),
							vec4(0,0,0,1)
		);

		mat4 roty = rotationy(gamer);
		mat4 rotx = rotationx(gamer);
		
		mat4 rot = roty * rotx;
		
		//t_pos = rotationx(-pi/4.0f) * t_pos;
		
		mat4 model = scale * rot * translation;
		//mat4 model = translation * rot * scale;
		mat4 view = lookat(vec3(0,0,1), vec3(0,0,-1), vec3(0,1,0));
		mat4 proj = perspectiveprojection(pi/2.0f, 4.0f/3.0f, 0.1f, 100.0f);
		//mat4 mvp = model * view * proj;
		//t_pos = model * t_pos;
		//t_pos = model * view * proj * t_pos;
		// t_pos = mvp * t_pos;
		t_pos = model * t_pos;
		t_pos = transform(t_pos, view);
		t_pos = transform(t_pos, proj);

	    t_pos.x /= t_pos.w;
	    t_pos.y /= t_pos.w;
	    t_pos.z /= t_pos.w;
		// t_pos.x += 1;    
		// t_pos.y += 1;
		//t_pos.z += 1.0f;
		


		// t_pos.x *= GetWidth() / 2.0f;
		// t_pos.y *= GetHeight() / 2.0f;

		inVert.pos = vec3(t_pos.x, t_pos.y, t_pos.z);

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


    Pixel colors[12];
	for (size_t i = 0; i < 12; i+=3)
	{
		colors[i]	= Pixel{254-i*4, 254, 254-i*4, 255};
		colors[i+1] = Pixel{0+i*2, 254-i*2, 254, 255};
		colors[i+2] = Pixel{0+i*3, i*4, i*5, 255};
	}
	int j = 0;
	for (size_t i = 0; i < object.i_buffer.size(); i+=3)
	{

		vec3 points[3];
		Vertex verts[3];
		verts[0] = object.v_buffer[object.i_buffer[i]];
		verts[1] = object.v_buffer[object.i_buffer[i+1]];
		verts[2] = object.v_buffer[object.i_buffer[i+2]];
		vertex_shader(verts[0]);
		vertex_shader(verts[1]);
		vertex_shader(verts[2]);

		points[0] = verts[0].pos;
		points[1] = verts[1].pos;
		points[2] = verts[2].pos;

		if (Cull(points[0],points[1],points[2]))
			continue;
		ToScreenSpace(points[0]);
		ToScreenSpace(points[1]);
		ToScreenSpace(points[2]);
		
		// if (Cull(points[0],points[1],points[2]))
		// 	continue;
		

		randomcolor = 12*(i+1);
		//WireFrame(points[0], points[1], points[2]);
		//BarRasterizeTriangle(points, Pixel{254, 254, 254, 254});
		//RasterizeTriangle(points[0], points[1]S, points[2], Pixel{254, 254, 254, 254});
		//TriangleRaster(points[0], points[1], points[2], Pixel{255*randomcolor,255*randomcolor,255*randomcolor,255}/*Pixel{rand()%255, rand()%255, rand()%255, 255}*/);
		//TriangleRaster(points[0], points[1], points[2], Pixel{40*(randomcolor/12),randomcolor,randomcolor,255}/*Pixel{rand()%255, rand()%255, rand()%255, 255}*/);
		TriangleRaster(points[0], points[1], points[2], colors[j]);
		j++;
		//NoCullBarRasterizeTriangle(points, Pixel{rand()%255, rand()%255, rand()%255, 255});
	}

	gamer += 0.01f;

	// in (-1,-1) to (1,1)
	// vec3 center(0.0f, 0.0f);
	// vec3 right(1.f, 0.0f);
	// vec3 center1(0.0f, 0.5f);
	// vec3 right1(1.f, 0.5f);
	// ToScreenSpace(center);
	// ToScreenSpace(right);
	// ToScreenSpace(right1);
	// ToScreenSpace(center1);
	// DrawLine(center, right);
	// DrawLine(center1, right1);
	
	

	

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
	for (size_t i = width*height; i--; depth_buffer[i] = -std::numeric_limits<float>::max());

	
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

void Renderer::SetVertextShader(std::function<void(Vertex&)> vertex_lambda)
{
	this->vertex_shader = vertex_lambda;
}

void Renderer::SetFragmentShader(std::function<Pixel(Vertex)> frag_lambda)
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
		//loader.LoadedVertices[i].pos *= 0.5f;
		//loader.LoadedVertices[i].pos.z += 1.f;

		outVerts.emplace_back(loader.LoadedVertices[i]);
	}
	
	AddBuffer(outVerts, outIndices, 0);

	
	return true;
}
//bounding box barycoord raster
void Renderer::BarRasterizeTriangle(vec3* points, Pixel colour)
{

	//setup Bounding box from vertices

	vec3 boundingboxMIN(fb_width - 1, fb_height - 1);
	vec3 boundingboxMAX(0, 0);
	vec3 clamped(fb_width -1, fb_height - 1);
	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 2; j++)
		{
			// TODO: static cast, ugly --> change matlib in future for different vec data-types i.e float, int
			boundingboxMIN[j] = std::max(0.0f, std::min(boundingboxMIN[j], points[i][j]));
			boundingboxMAX[j] = std::min(clamped[j], std::max(boundingboxMAX[j], points[i][j]));
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
				P.z += points[i][2] * screenBarycentric[i];
			}

			if (depth_buffer[int(P.x + P.y * fb_width)] < P.z) // check depth, discard otherwise
			{
				depth_buffer[int(P.x + P.y * fb_width)] =  P.z;
				PlacePixel(P.x, P.y, colour);
			}
			
		}
	}
	
}
void Renderer::NoCullBarRasterizeTriangle(vec3* pts, Pixel colour)
{
	// for (size_t ii = 0; ii < 3; ii++)
	// {	
	// 	float temp_x, temp_y;
	// 	temp_x = (pts[ii].x) * fb_width / 2.0f;
	// 	temp_y = (pts[ii].y) * fb_height / 2.0f;
	// 	pts[ii].x = temp_x;
	// 	pts[ii].y = temp_y;
	// }

	vec3 bboxmin( std::numeric_limits<float>::max(),  std::numeric_limits<float>::max());
    vec3 bboxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
    vec3 clamp(fb_width-1, fb_height-1);
    for (int i=0; i<3; i++) {
        for (int j=0; j<2; j++) {
            bboxmin[j] = std::max(0.f,      std::min(bboxmin[j], pts[i][j]));
            bboxmax[j] = std::min(clamp[j], std::max(bboxmax[j], pts[i][j]));
        }
    }
    vec3 P;
    for (P.x=bboxmin.x; P.x<=bboxmax.x; P.x++) {
        for (P.y=bboxmin.y; P.y<=bboxmax.y; P.y++) {
            vec3 bc_screen  = barycentric(pts, P);
            if (bc_screen.x<0 || bc_screen.y<0 || bc_screen.z<0) {continue; printf("gamer \n");}
			
                PlacePixel(P.x, P.y, colour);
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
		frame_buffer[i].r = 254;
		frame_buffer[i].g = 0; 
		frame_buffer[i].b = 0; 
		frame_buffer[i].a = 254; 

	}

	
}
void Renderer::WireFrame(vec3 v0, vec3 v1, vec3 v2)
{
	
	// v0.x *= GetWidth() / 2;
	// v0.y *= GetHeight() / 2;
	// v1.x *= GetWidth() / 2;
	// v1.y *= GetHeight() / 2;
	// v2.x *= GetWidth() / 2;
	// v2.y *= GetHeight() / 2;
	DrawLine(v0, v1);
	DrawLine(v1, v2);
	DrawLine(v2, v0);
}



void Renderer::TriangleRaster(const vec3& v0, const vec3& v1, const vec3& v2, Pixel color)
{
	
	const vec3* p_v0 = &v0;
	const vec3* p_v1 = &v1;
	const vec3* p_v2 = &v2;

	// sort height of vertices
	if (p_v1->y < p_v0->y)
		std::swap(p_v0, p_v1);
	if (p_v2->y < p_v1->y)
		std::swap(p_v1, p_v2);
	if (p_v1->y < p_v0->y)
		std::swap(p_v0, p_v1);
	
	// identify the type of triangle, top-  bot- right- left- major

	if (p_v0->y == p_v1->y)	// top flat
	{
		// sort vertices by x-coord
		if (p_v1->x < p_v0->x)
			std::swap(p_v0, p_v1);

		FlatTopTriangle(*p_v0, *p_v1, *p_v2, color);
	}
	else if (p_v1->y == p_v2->y)	// bottom flat
	{
		// sort vertices by x-coord
		if (p_v2->x < p_v1->x)
			std::swap(p_v1, p_v2);
		
		FlatBottomTriangle(*p_v0, *p_v1, *p_v2, color);
	}
	else // any triangle
	{
		// find top bottom split vertex
		float k_split = (p_v1->y - p_v0->y) / (p_v2->y - p_v0->y);
		vec3 vK = *p_v0 + (*p_v2 - *p_v0) * k_split;

		if (p_v1->x < vK.x) // right major
		{
			FlatBottomTriangle(*p_v0, *p_v1, vK, color);
			FlatTopTriangle(*p_v1, vK, *p_v2, color);
		}
		else	// left major
		{
			FlatBottomTriangle(*p_v0, vK, *p_v1, color);
			FlatTopTriangle(vK, *p_v1, *p_v2, color);
		}
		
	}
	
}

void Renderer::FlatTopTriangle(const vec3& v0, const vec3& v1, const vec3& v2, Pixel color)
{


	// calc line slopes in screen-space
	float m0 = (v2.x - v0.x) / (v2.y - v0.y);
	float m1 = (v2.x - v1.x) / (v2.y - v1.y);

	// start and end for scanlines
	int scan_start = (int)ceil(v0.y - 0.5f);
	int scan_end = (int)ceil(v2.y - 0.5f);

	for (size_t y = scan_start; y < scan_end; y++)
	{
		// scanline start X
		float px0 = m0 * (float(y) + 0.5f - v0.y) + v0.x;
		float px1 = m1 * (float(y) + 0.5f - v1.y) + v1.x;

		// start, end pixels
		int pix_start = (int)ceil(px0 - 0.5f);
		int pix_end = (int)ceil(px1 - 0.5f);

		for (size_t x = pix_start; x < pix_end; x++)
		{
			
			PlacePixel(x, y, color);
		}
		
	}
	

}

void Renderer::FlatBottomTriangle(const vec3& v0, const vec3& v1, const vec3& v2, Pixel color)
{

	// calc line slopes in screen-space
	float m0 = (v1.x - v0.x) / (v1.y - v0.y);
	float m1 = (v2.x - v0.x) / (v2.y - v0.y);

	// start and end for scanlines
	int scan_start = (int)ceil(v0.y - 0.5f);
	int scan_end = (int)ceil(v2.y - 0.5f);

	for (size_t y = scan_start; y < scan_end; y++)
	{
		// scanline start X
		float px0 = m0 * (float(y) + 0.5f - v0.y) + v0.x;
		float px1 = m1 * (float(y) + 0.5f - v0.y) + v0.x;

		// start, end pixels
		int pix_start = (int)ceil(px0 - 0.5f);
		int pix_end = (int)ceil(px1 - 0.5f);

		for (size_t x = pix_start; x < pix_end; x++)
		{
			
			PlacePixel(x, y, color);
		}
		
	}
}

void Renderer::PutPixel(unsigned int x, unsigned int y, Pixel pix)
{
	assert(x >= 0);
	assert(y >= 0);
	assert(x < fb_width);
	assert(y < fb_height);
	this->frame_buffer[y*fb_width + x] = pix;
}

vec3& Renderer::ToScreenSpace(vec3& vec)
{
	vec.x = (vec.x + 1.0f) * width_offset; 
	vec.y = (vec.y + 1.0f) * height_offset; 
	return vec;
}


bool Renderer::Cull(vec3 v0, vec3 v1, vec3 v2) const
{
	// project on surface normal
	return dot(cross((v1 - v0), (v2 - v0)), v0) > 0;
}