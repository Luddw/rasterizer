#include "meshresource.h"
#include <utility>
#include <fstream>
#include <sstream>
#include <map>



MeshResource::MeshResource(std::vector<Vertex> vertzz, std::vector<GLuint> indices)
{
	this->vertexss = std::move(vertzz);
	this->indices = std::move(indices);

}

MeshResource::~MeshResource()
{
	glDeleteBuffers(1,&ibo);
	glDeleteBuffers(1, &vbo);
	
	std::cout << " MESH DESTRUCTOR CALLED " << std::endl;
}


MeshResource::MeshResource()
{
	//vert.clear();
	//indices.clear();
	//vertexss.clear();
}

void MeshResource::SetupIndexBuffer()
{
    glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    const GLuint buffer_size = indices.size() *sizeof(GLuint);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,buffer_size, &indices[0], GL_STATIC_DRAW);
}

void MeshResource::SetupVertexBuffer()
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	/*const GLuint buffer_size =;*/
	glBufferData(GL_ARRAY_BUFFER, vertexss.size() * sizeof(Vertex), &vertexss[0].pos, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, sizeof(Vertex), NULL);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uvPos));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		  

}
void MeshResource::SetupVertexArray()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindVertexArray(0);
}

void MeshResource::BindVao() const
{
    glBindVertexArray(vao);
}

void MeshResource::BindIbo() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
}
void MeshResource::BindVbo() const
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
}
void MeshResource::UnBindVao()
{
	glBindVertexArray(0);
}

void MeshResource::DrawMesh()
{
	BindVbo();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, sizeof(Vertex), NULL);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uvPos));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	BindIbo();
}

void MeshResource::UnBindIbo()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void MeshResource::UnBindVbo()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void MeshResource::DrawCube(float size) 
{
	size /= 2;

	vertexss = { 
		// Left
		Vertex(Vector4D(-size, -size, -size),	Vector4D(1.0f, 0.75f),Vector4D(-1,0,0)),
		Vertex(Vector4D(-size, -size, size),   Vector4D(0.75f, 0.75f),Vector4D(-1,0,0)),
		Vertex(Vector4D(-size, size, size),		Vector4D(0.75f, 0.5f),Vector4D(-1,0,0)),
		Vertex(Vector4D(-size, size, -size),	 Vector4D(1.0f, 0.5f),Vector4D(-1,0,0)),

		// Front
		Vertex(Vector4D(-size,-size,size),	  Vector4D(0.25f, 0.75f),Vector4D(0,0,1)),
		Vertex(Vector4D(size, -size, size),	   Vector4D(0.0f, 0.75f),Vector4D(0,0,1)),
		Vertex(Vector4D(size, size, size),	    Vector4D(0.0f, 0.5f),Vector4D(0,0,1)),
		Vertex(Vector4D(-size, size, size),	   Vector4D(0.25f, 0.5f),Vector4D(0,0,1)),



		// Back
		Vertex(Vector4D(size, -size, -size), Vector4D(0.75f, 0.75f),Vector4D(0,0,-1)),
		Vertex(Vector4D(-size, -size, -size), Vector4D(0.5f, 0.75f),Vector4D(0,0,-1)),
		Vertex(Vector4D(-size, size, -size),   Vector4D(0.5f, 0.5f),Vector4D(0,0,-1)),
		Vertex(Vector4D(size, size, -size),   Vector4D(0.75f, 0.5f),Vector4D(0,0,-1)),



		// Right
		Vertex(Vector4D(size, -size, size),Vector4D(0.5f, 0.75f),  Vector4D(1,0,0)),
		Vertex(Vector4D(size, -size, -size),Vector4D(0.25f, 0.75f),Vector4D(1,0,0)),
		Vertex(Vector4D(size, size, -size),Vector4D(0.25f, 0.5f),  Vector4D(1,0,0)),
		Vertex(Vector4D(size, size, size),  Vector4D(0.5f, 0.5f),  Vector4D(1,0,0)),

		// Top
		Vertex(Vector4D(-size, size, size), Vector4D(0.75f, 0.5f),  Vector4D(0,1,0)),
		Vertex(Vector4D(size, size, size),  Vector4D(0.5f, 0.5f),   Vector4D(0,1,0)),
		Vertex(Vector4D(size, size, -size), Vector4D(0.5f, 0.25f),  Vector4D(0,1,0)),
		Vertex(Vector4D(-size, size, -size), Vector4D(0.75f, 0.25f),Vector4D(0,1,0)),

		// Bottom
		Vertex(Vector4D(size, -size, size),  Vector4D(0.75f, 1.0f),  Vector4D(0,-1,0)),
		Vertex(Vector4D(-size, -size, size), Vector4D(0.5f, 1.0f),   Vector4D(0,-1,0)),
		Vertex(Vector4D(-size, -size, -size),Vector4D(0.5f, 0.75f),  Vector4D(0,-1,0)),
		Vertex(Vector4D(size, -size, -size),  Vector4D(0.75f, 0.75f),Vector4D(0,-1,0)),
	};
	indices = {
	0,1,3,		//triangle 1 //front
	2,3,1,		//triagnle 2

	4,5,7,		//triangle 1 //back		
	6,7,5,		//triagnle 2

	8,9,11,		//triangle 1 //right		
	10,11,9,		//triagnle 2

	12,13,15,		//triangle 1				
	14,15,13,		//triagnle 2

	16,17,19,	//triangle 1				
	18,19,17,		//triagnle 2

	20,21,23,		//triangle 1				
	22,23,21,	//triagnle 2



	};

	
	SetupVertexBuffer();
	SetupIndexBuffer();
	
	/*m.UnBindIbo();
	m.UnBindVbo();*/
}


void MeshResource::ObjLoad(const char* filepath)
{
	//std::vector<Vector4D> &file_verts;
	//std::vector<Vector4D> &file_uvs;
	//std::vector<Vector4D> &file_norms;

	std::vector<GLuint> vertexIndices, uvIndices, normIndices;
	std::vector<Vector4D> t_verts;
	std::vector<Vector4D> t_uvs;
	std::vector<Vector4D> t_norms;

	std::ifstream stream(filepath);
	std::string line;

	enum type
	{
		v, vt, vn, f, none
	};
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
			Vector4D vert;
			for (size_t i = 1; i < 4; i++)
			{
				scanf(tokens[i].c_str(), "%f", &vert[i - 1]);
			}

			t_verts.emplace_back(vert);
			break;
		}
		case vt:
		{
			Vector4D uv(0,0);
			for (size_t i = 1; i < 3; i++)
			{
				scanf(tokens[i].c_str(), "%f", &uv[i - 1]);

			}


			t_uvs.emplace_back(uv);
			break;
		}
		case vn:
		{
			Vector4D norm;
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
	std::vector<Vector4D> buf;
	vertexss.clear();
	vert.clear();
	indices.clear();

	for (size_t i = 0; i < vertexIndices.size(); i++)
	{
		unsigned int vertIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normIndex = normIndices[i];

	
		Vector4D vertex = t_verts[vertIndex - 1];
		Vector4D uv = t_uvs[uvIndex - 1];
		Vector4D norm = t_norms[normIndex - 1];
		
		vertexss.emplace_back(Vertex(vertex, uv, norm));
		indices.emplace_back(i);
	}

	std::cout << vertexIndices.size() << std::endl;
	std::cout << uvIndices.size() << std::endl;
	std::cout << normIndices.size() << std::endl;

	//indices = vertexIndices;
	SetupVertexBuffer();
	SetupIndexBuffer();


}