#pragma once
#include "mat4.h"
#include "matlib.h"
#include <GL/glew.h>
#include <vector>



struct Vertex
{
	Vertex() : pos(vec3()), uv(vec3()) { };
	Vertex(const Vertex& other) : pos(other.pos), uv(other.uv)/*, normal(other.normal) */{ };
	Vertex(vec3 position, vec3 uv, vec3 norm) : pos(position), uv(uv), normal(norm) { };
	//Vertex(const vec3& position, const vec3& uv/*, const vec3& norm*/): pos(position), uvPos(uv)/*,normal(norm)*/{};
	vec3 pos;
	vec3 uv;
    vec3 normal;
	
};


/** A class that represents a Mesh in OpenGL*/
class MeshResource
{
public:
	/** 
	* Custom constructor for creating a mesh
	* @param verts: a vector containing GLfloats for the setup of vertexbuffer and vertexattribs
	* @param indices: a vector containing GLuints for the setup of indexbuffer
	*/
	MeshResource();
	MeshResource(std::vector<Vertex> vertzz, std::vector<GLuint> indices);

	/** stores each vertices value for use in the mesh methods */
	/** stores each indices value for use in the mesh methods */
	~MeshResource();
	std::vector<GLuint> indices;
	std::vector<Vertex> vertexss;
	std::vector<vec3> vert;
	/** method to bind the Vertex array object*/
	void BindVao() const;
	/** method to bind the Vertex buffer object*/
	void BindVbo() const;
	/** method to bind the Index buffer object */
	void BindIbo() const;

	void SetupMeshResource();
	void UnBindIbo();
	void UnBindVbo();
	void UnBindVao();
	void DrawMesh();
	void DrawCube(float size);
	void ObjLoad(const char* filepath);
	/** 
	* Generates buffers and binds them, Buffers the data to the gpu.
	* Enables Vertex attributes, position and colour for each vertex.
	*/
	void SetupVertexBuffer();
	/** Generates buffers and binds them, Buffers the data to the gpu. */
	void SetupIndexBuffer();
	/** Generates A vertex array, and binds it. */
	void SetupVertexArray();
	

private:
	/** Unsigned ints used in openGL to represent each object */
	GLuint vao{},vbo{},ibo{};
	
};