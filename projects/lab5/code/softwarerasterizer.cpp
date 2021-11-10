#include <functional>
#include <cmath>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <time.h>
#include <queue>
#include "softwarerasterizer.h"

static float staticRotation = 0;
Renderer::Renderer() : fb_width(0), fb_height(0), frame_buffer(nullptr), depth_buffer(nullptr)
{

}

Renderer::Renderer(const int width, const int height)
{
	fb_width = width;
	fb_height = height;
	height_offset = height / 2.0f;    
	width_offset = width / 2.0f;    
	SetupFrameBuffer(width, height);
	model = OBJLoader("./resources/cube.obj");
	Texture meshTex("./resources/error.png");
	texture = meshTex;  



	SetVertextShader([&](Vertex inVert) -> VertexOut {
		vec4 t_pos(inVert.pos.x, inVert.pos.y, inVert.pos.z, 1.0);
		

		mat4 translation(	1,0,0,0,
							0,1,0,0,
							0,0,1,0,
							0,0,0,1

		);
		mat4 scale(

							vec4(0.4,0,0,0),
							vec4(0,0.4,0,0),
							vec4(0,0,0.4,0),
							vec4(0,0,0,1)
		);
		mat4 roty = rotationy(staticRotation);
		mat4 rotx = rotationx(staticRotation);
		
		mat4 rot = roty * rotx;
		
		mat4 model = scale * rot * translation;

		mat4 view = lookat(vec3(0,0,1), vec3(0,0,-1), vec3(0,1,0));
		mat4 proj = perspectiveprojection(pi/2.0f, 4.0f/3.0f, 0.1f, 100.0f);

		t_pos = transform(t_pos, model);
		t_pos = transform(t_pos, view);
		t_pos = transform(t_pos, proj);


		vec4 modelspacenormal = transpose(inverse(model)) * vec4(inVert.normal.x, inVert.normal.y, inVert.normal.z, 1.0f);
		vec3 mspacenormal(modelspacenormal);

		vec4 fragPos = model * vec4(inVert.pos.x, inVert.pos.y, inVert.pos.z, 1.0f);

		return {t_pos, inVert.uv, mspacenormal, fragPos};
	});

	SetFragmentShader([&](VertexOut& inVert, Texture& tex) -> Pixel {
		//vec3 lightColor(0.9,0.9,0.9);
		vec3 lightColor(0.3,0.3,0.3);
		
		
		vec3 lightPos(1,0.2,1);
		vec3 viewPos(0,0,1);

		Pixel texcol = /*Pixel{0,0,255,255};*/ tex.GetColor(inVert.uv);
		texcol = Pixel{0,0,255,255};
		vec3 col = vec3(texcol.r/255.0f,texcol.g/255.0f, texcol.b/255.0f);
		vec3 ambient = col * 0.05;

		vec3 normal = normalize(inVert.normal);
		vec3 lightDir = normalize(lightPos - inVert.frag_pos);
		float diff = fmax(dot(lightDir, normal), 0.0f);
		vec3 diffuse = lightColor * diff;

		vec3 viewDir = normalize(viewPos - inVert.frag_pos);
		vec3 reflectDir = reflect(-lightDir, normal);
		float spec = 0.0f;

		vec3 halfwayDir = normalize(lightDir + viewDir);
		spec = pow(fmax(dot(normal, halfwayDir), 0.0f), 32.0);
		vec3 specular = (lightColor * spec) * spec;


		vec3 ambDiffSpec = ambient + diffuse + specular;
		ambDiffSpec = mul(ambDiffSpec, col);
		vec4 FragCol = vec4(ambDiffSpec.x, ambDiffSpec.y, ambDiffSpec.z, 1.0f);

		Pixel outColor{FragCol.x*255, FragCol.y*255, FragCol.z*255, FragCol.w *255};

		return outColor;
	});
}

Renderer::~Renderer()
{
}


void Renderer::Draw(unsigned int handle)
{

	std::vector<Vertex> mesh = GetMesh();
	VertexOut* outVert = new VertexOut[mesh.size()];

	for (size_t i = 0; i < mesh.size(); i++)
	{
		outVert[i] = vertex_shader(mesh[i]);
	}
	for (size_t i = 0; i < mesh.size(); i+=3)
	{
		if (Cull(outVert[i].pos, outVert[i+1].pos, outVert[i+2].pos))
			continue;

		ToScreenSpace(outVert[i].pos);
		ToScreenSpace(outVert[i+1].pos);
		ToScreenSpace(outVert[i+2].pos);

		TriangleRaster(outVert[i], outVert[i+1], outVert[i+2]);
	}
	
	staticRotation += 0.01f;
	delete[] outVert;
}


const unsigned int Renderer::AddBuffer(std::vector<Vertex> &vbuff, std::vector<unsigned int> &ibuff)
{
	buffer_handles.push_back(BufferObject(vbuff, ibuff));	
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

void Renderer::UpdateQuadTex(GLuint handle)
{
	glBindTexture(GL_TEXTURE_2D, handle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexSubImage2D(GL_TEXTURE_2D,0,0,0,fb_width,fb_height,GL_RGBA,GL_UNSIGNED_BYTE, frame_buffer);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::ClearFB()
{
	for (size_t i = 0; i < fb_height*fb_width; i++)
	{
		frame_buffer[i].r = 0;
		frame_buffer[i].g = 0; 
		frame_buffer[i].b = 0; 
		frame_buffer[i].a = 254; 
		depth_buffer[i] = -std::numeric_limits<float>::infinity();
	}
}

void Renderer::WireFrame(vec3 v0, vec3 v1, vec3 v2)
{
	DrawLine(v0, v1);
	DrawLine(v1, v2);
	DrawLine(v2, v0);
}

void Renderer::TriangleRaster(const VertexOut& v0, const VertexOut& v1, const VertexOut& v2)
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

		FlatTopTriangle(*p_v0, *p_v1, *p_v2);
	}
	else if (p_v1->pos.y == p_v2->pos.y)	// bottom flat
	{
		// sort vertices by x-coord
		if (p_v2->pos.x < p_v1->pos.x)
			std::swap(p_v1, p_v2);
		
		FlatBottomTriangle(*p_v0, *p_v1, *p_v2);
	}
	else // any triangle
	{
		// find top bottom split vertex
		float k_split = (p_v1->pos.y - p_v0->pos.y) / (p_v2->pos.y - p_v0->pos.y);
		VertexOut vk = p_v0->Interpolate(*p_v2, k_split);

		if (p_v1->pos.x < vk.pos.x) // right major
		{
			FlatBottomTriangle(*p_v0, *p_v1, vk);
			FlatTopTriangle(*p_v1, vk, *p_v2);
		}
		else	// left major
		{
			FlatBottomTriangle(*p_v0, vk, *p_v1);
			FlatTopTriangle(vk, *p_v1, *p_v2);
		}
	}
}

void Renderer::FlatTopTriangle(const VertexOut& v0, const VertexOut& v1, const VertexOut& v2)
{
	// calc line slopes in screen-space
	float m0 = (v2.pos.x - v0.pos.x) / (v2.pos.y - v0.pos.y);
	float m1 = (v2.pos.x - v1.pos.x) / (v2.pos.y - v1.pos.y);

	// start and end for scanlines
	int scan_start = (int)ceil(v0.pos.y );
	int scan_end = (int)ceil(v2.pos.y );

	VertexOut P;
	for (int y = scan_start; y < scan_end; y++)
	{
		// scanline start X
		float px0 = m0 * (float(y)  - v0.pos.y) + v0.pos.x;
		float px1 = m1 * (float(y) - v1.pos.y) + v1.pos.x;

		// start, end pixels
		int pix_start = (int)ceil(px0 );
		int pix_end = (int)ceil(px1 );

		for (int x = pix_start; x < pix_end; x++)
		{
			P.pos.x = x;
			P.pos.y = y;
			vec3 weights = barycentric(v0.pos, v1.pos, v2.pos, P.pos);

			P = ApplyWeights(v0, v1, v2, weights);
			P.pos.x = x;
			P.pos.y = y;

			if (depth_buffer[int(x + y * fb_width)] < P.pos.z)
			{
				depth_buffer[int(x + y * fb_width)] = P.pos.z;
				PlacePixel(P.pos.x, P.pos.y, frag_shader(P, texture));
			}
		}
	}
}

void Renderer::FlatBottomTriangle(const VertexOut& v0, const VertexOut& v1, const VertexOut& v2)
{
	// calc line slopes in screen-space
	float m0 = (v1.pos.x - v0.pos.x) / (v1.pos.y - v0.pos.y);
	float m1 = (v2.pos.x - v0.pos.x) / (v2.pos.y - v0.pos.y);

	// start and end for scanlines
	int scan_start = (int)ceil(v0.pos.y );
	int scan_end = (int)ceil(v2.pos.y );

	VertexOut P;
	for (int y = scan_start; y < scan_end; y++)
	{
		// scanline start X
		float px0 = m0 * (float(y) - v0.pos.y) + v0.pos.x;
		float px1 = m1 * (float(y)  - v0.pos.y) + v0.pos.x;

		// start, end pixels
		int pix_start = (int)ceil(px0 );
		int pix_end = (int)ceil(px1 );

		for (int x = pix_start; x < pix_end; x++)
		{

			P.pos.x = x;
			P.pos.y = y;
			vec3 weights = barycentric(v0.pos, v1.pos, v2.pos, P.pos);

			P = ApplyWeights(v0, v1, v2, weights);
			P.pos.x = x;
			P.pos.y = y;

			if (depth_buffer[int(x + y * fb_width)] < P.pos.z)
			{
				depth_buffer[int(x + y * fb_width)] = P.pos.z;
				PlacePixel(P.pos.x, P.pos.y, frag_shader(P, texture));
			}
		}
	}
}

VertexOut Renderer::ApplyWeights(VertexOut v0, VertexOut v1, VertexOut v2, vec3 weights)
{
	VertexOut out;
	out.pos = v0.pos * weights.x + v1.pos * weights.y + v2.pos * weights.z;
	out.pos.z = 0;
	out.pos.z += v0.pos.z * weights.x + v1.pos.z * weights.y + v2.pos.z * weights.z;
	out.uv = v0.uv * weights.x + v1.uv * weights.y + v2.uv * weights.z;
	out.normal = v0.normal * weights.x + v1.normal * weights.y + v2.normal * weights.z;
	out.frag_pos = v0.frag_pos * weights.x + v1.frag_pos * weights.y + v2.frag_pos * weights.z;

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

std::vector<Vertex> Renderer::OBJLoader(const char* filepath)
{
	std::vector<vec3> positions;
	std::vector<vec3> texcoords;
	std::vector<vec3> normals;

	std::vector<unsigned int> posIndices;
	std::vector<unsigned int> uvIndices;
	std::vector<unsigned int> normalIndices;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	std::stringstream ss;
	std::ifstream fileStream(filepath);
	std::string line = "";
	std::string attrib = "";
	vec3 tempVec3;
	unsigned int tempUint = 0;


	while (std::getline(fileStream, line))
	{
		ss.clear();
		ss.str(line);
		ss >> attrib;

		if (attrib == "v")
		{
			ss >> tempVec3.x >> tempVec3.y >> tempVec3.z;
			positions.push_back(tempVec3);
		}
		else if (attrib == "vt")
		{
			ss >> tempVec3.x >> tempVec3.y >> tempVec3.z;
			texcoords.push_back(tempVec3);
		}
		else if (attrib == "vn")
		{
			ss >> tempVec3.x >> tempVec3.y >> tempVec3.z;
			normals.push_back(tempVec3);
		}
		else if (attrib == "f")
		{
			int count = 0;
			while (ss >> tempUint)
			{
				if(count == 0)
					posIndices.push_back(tempUint);
				else if(count == 1)
					uvIndices.push_back(tempUint);
				else if(count == 2)
					normalIndices.push_back(tempUint);
				
				if(ss.peek() == '/')
				{
					++count;
					ss.ignore(1, '/');
				}
				else if (ss.peek() == ' ')
				{
					++count;
					ss.ignore(1, ' ');
				}
				
				if(count > 2)
					count = 0;
			}
		} 

		vertices.resize(posIndices.size(), Vertex());

		for (size_t i = 0; i < vertices.size(); i++)
		{
			vertices[i].pos = positions[posIndices[i]-1];
			vertices[i].uv = texcoords[uvIndices[i]-1];
			vertices[i].normal = normals[normalIndices[i]-1];
			
		}
	}

	return vertices;
}

std::vector<Vertex> Renderer::GetMesh()
{
	return this->model;
}