#include "shaderresource.h"
#include "matlib.h"
#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


ShaderResource::ShaderResource(const std::string& file)
: path(file), shader_id(0)
{
	const ShaderProgram src = Parser(file);
	shader_id = CreateShader(src.vs,src.fs);
}
ShaderResource::~ShaderResource()
{
	glDeleteProgram(shader_id);
}

ShaderProgram ShaderResource::Parser(const std::string &path) const
{
	std::ifstream stream(path);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream stringStream[2];
	ShaderType type = ShaderType::NONE;

	while (getline(stream, line)) 
	{
		if (line.find("#shader") != std::string::npos) // finds "#shader <<type of shader>>" in file
		{
			if (line.find("vertex") != std::string::npos) // mode set to vertex , found "#shader vertex"
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos) // mode set to fragment, found "#shader frag"
				type = ShaderType::FRAGMENT;
		}
		else
		{
			stringStream[static_cast<int>(type)] << line << '\n'; // if anything else, add it to appropriate string stream
		}
	}

	return { stringStream[0].str(), stringStream[1].str() };
}


unsigned int ShaderResource::CompShader(const unsigned int type, const std::string &source)
{
	const unsigned int id = glCreateShader(type);
	const char * src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);//error handling, if shader fails to compile
	if (result == GL_FALSE) {
		int lenght;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght);
		char* message = static_cast<char*>(alloca(sizeof(char) * lenght));
		glGetShaderInfoLog(id, lenght, &lenght, message);
		std::cout << "failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " SHADER" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}
	return id;
}



unsigned int ShaderResource::CreateShader(const std::string & vert_shader, const std::string & frag_shader)
{
	const unsigned int program = glCreateProgram();
	const unsigned int vs = CompShader(GL_VERTEX_SHADER, vert_shader);
	const unsigned int fs = CompShader(GL_FRAGMENT_SHADER, frag_shader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);
	
	return program;
}


void ShaderResource::Bind() const
{
	glUseProgram(shader_id);
}
void ShaderResource::UnBind()
{
	glUseProgram(0);
}

void ShaderResource::SetUniform4f(const std::string& name, Vector4D vect)
{
	glUniform4f(GetUniLocation(name), vect[0], vect[1], vect[2], vect[3]);
}

void ShaderResource::SetUniformMatrix4fv(const std::string& name, Matrix4D mat)
{
	glUniformMatrix4fv(GetUniLocation(name), 1, GL_TRUE, &(mat)[0]);
}

void ShaderResource::SetUniformFloat(const std::string& name, float value)
{
	glUniform1f(GetUniLocation(name), (GLfloat)value);
	
}

int ShaderResource::GetUniLocation(const std::string &name)
{
	if (uniformCache.find(name)!= uniformCache.end()) //checks if uniform already exists in cache
		return uniformCache[name];

	const int location = glGetUniformLocation(shader_id,name.c_str());

	if(location ==-1) //file not found
		std::cout<<name<< " doesn't exist" << std::endl;

	uniformCache[name] = location; //adds uniform to cache
	
	return location;
}