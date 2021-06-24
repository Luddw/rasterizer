#pragma once
#include <unordered_map>
#include <string>
struct ShaderProgram
{
	std::string vs;
	std::string fs;
};
class Vector4D;
class Matrix4D;

class ShaderResource
{
private:
	std::string path;
	unsigned int shader_id;
	std::unordered_map<std::string, int> uniformCache;
	int GetUniLocation(const std::string &name);
	unsigned int CreateShader(const std::string & vert_shader, const std::string & frag_shader);
	unsigned int CompShader(unsigned int type, const std::string &source);

	ShaderProgram Parser(const std::string &path) const;
public:
	ShaderResource(const std::string& file);
	~ShaderResource();


	void Bind() const; // gl use program
	void UnBind();

	//uniforms
	void SetUniform4f(const std::string &name, Vector4D vect);
	void SetUniformMatrix4fv(const std::string& name, Matrix4D mat);
	void SetUniformFloat(const std::string& name, float value);
	void SetUniformTex(const std::string& name, int value);

};

