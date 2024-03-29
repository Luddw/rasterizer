#include "graphicnode.h"
#include "pointlight.h"


static bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << ")" << function <<
			" " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}

GraphicNode::GraphicNode(MeshResource mesh, Texture tex, ShaderResource shader, Transform trans, Cam cam)
{
	p_Mesh = std::make_shared<MeshResource>(mesh);
	p_Shader = std::make_shared<ShaderResource>(shader);
	p_Texture = std::make_shared<Texture>(tex);
	p_Transform = std::make_shared<Transform>(trans);
	p_Cam = std::make_shared<Cam>(cam);
	lightsrc = nullptr;
}


GraphicNode::GraphicNode() :
	p_Mesh(nullptr), p_Texture(nullptr), p_Shader(nullptr), p_Transform(nullptr), p_Cam(nullptr), lightsrc(nullptr)
{
}

GraphicNode::~GraphicNode()
{
}

GraphicNode::GraphicNode(std::shared_ptr<MeshResource> mesh, std::shared_ptr<Texture> texture,
	std::shared_ptr<ShaderResource> shader, std::shared_ptr<Transform> transform, std::shared_ptr<Cam> cam)
{

	p_Mesh = std::move(mesh);
	p_Texture = std::move(texture);
	p_Shader = std::move(shader);
	p_Transform = std::move(transform);
	p_Cam = std::move(cam);
}

void GraphicNode::Draw()
{
	p_Mesh->DrawMesh();
	p_Texture->Bind(0);
	p_Shader->Bind();
	p_Shader->SetUniformMatrix4fv("m", p_Transform->MakeModel());
	p_Shader->SetUniformMatrix4fv("vp", p_Cam->GetProj() * p_Cam->GetView());
	p_Shader->SetUniform4f("lightcolor", lightsrc->GetColor());
	p_Shader->SetUniform4f("lightpos", lightsrc->GetPos());
	p_Shader->SetUniform4f("intensity", Vector4D(lightsrc->GetIntensity(),0));
	p_Shader->SetUniform4f("camerapos", p_Cam->GetPos());


	glDrawElements(GL_TRIANGLES, p_Mesh->indices.size(), GL_UNSIGNED_INT, NULL);
	/*glDrawArrays(GL_TRIANGLES, 0, p_Mesh->vertexss.size());*/
	p_Mesh->UnBindIbo();
	p_Mesh->UnBindVbo();
	p_Texture->Unbind();
	p_Shader->UnBind();
}



void GraphicNode::SetMesh(MeshResource& newmesh)
{
	p_Mesh = std::make_shared<MeshResource>(newmesh);
}

void GraphicNode::SetShader(ShaderResource& newshad)
{
	p_Shader = std::make_shared<ShaderResource>(newshad);
}

void GraphicNode::SetTexture(Texture& newtex)
{
	p_Texture = std::make_shared<Texture>(newtex);
}

void GraphicNode::SetTransform(Transform& newtrans)
{
	p_Transform = std::make_shared<Transform>(newtrans);
}

std::shared_ptr<MeshResource> GraphicNode::GetMesh() const
{
	return p_Mesh;
}

std::shared_ptr<ShaderResource> GraphicNode::GetShader() 
{
	return p_Shader;
}

std::shared_ptr<Texture> GraphicNode::GetTexture() const
{
	return p_Texture;
}

std::shared_ptr<Transform> GraphicNode::GetTransform() const
{
	return p_Transform;
}

void GraphicNode::SetLight(PointLight &light)
{
	lightsrc = &light;
}

