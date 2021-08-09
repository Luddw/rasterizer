//------------------------------------------------------------------------------
// Lab5.cc
// (C) 2015-2018 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "lab5.h"
#include <sstream>
#include <GL/glew.h>
#include <iostream>
#include <fstream>
typedef int int32;
typedef double float64;
using namespace Display;


namespace Example
{
	//------------------------------------------------------------------------------
	/**
	*/
	Lab5::Lab5()
	{
		// empty
	}
	
	//------------------------------------------------------------------------------
	/**
	*/
	Lab5::~Lab5()
	{
		// empty
	}
	void Lab5::keyEvent(int key, int action, int modifier)
	{

	}
	void Lab5::MousePos(float xpos, float ypos)
	{

	}
	//------------------------------------------------------------------------------
	/**
	*/

	bool
	Lab5::Open()
	{
		App::Open();
		
		this->window = new Window;
		int w, h;
		this->window->GetSize(w, h);
		window->SetKeyPressFunction([this](int32 key, int32, int32 action, int32 mod)
		{
			this->keyEvent(key, action, mod);
			std::cout <<"Key: "<< key << " action: "<<action << std::endl;
			if (action == 1)	
			{
				
				switch (key)
				{
				case GLFW_KEY_D: {mainnode.GetTransform()->Move(-0.1f, 0, 0); break; }
				case GLFW_KEY_A: {mainnode.GetTransform()->Move(0.1f, 0, 0); break; }
				case GLFW_KEY_S: {mainnode.GetTransform()->Move(0, 0, 0.1f); break; }
				case GLFW_KEY_W: {mainnode.GetTransform()->Move(0, 0, -0.1f); break; }								 
				case GLFW_KEY_ESCAPE: {r.SaveFB(); break; }								 
				case GLFW_KEY_M: {glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); break; } //wireframe
				case GLFW_KEY_N: {glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); break; }
				case GLFW_KEY_H: {; break; }

				default: break;
				}
			}
		
			
		}
		);
		
		window->SetMousePressFunction([this](int32 key, int32 action, int32 mod)
		{
			this->keyEvent(key, action, mod);
			std::cout << "Key: " << key << " action: " << action << std::endl;

			if (key == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
			{
				leftButtonPressed = true;		
			}
			if (key == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
			{
				leftButtonPressed = false;
			}
		}
		);


		
		window->SetMouseMoveFunction([this](float64 xpos, float64 ypos)
			{
				this->MousePos(xpos, ypos);
				
				//std::cout << xpos << "  :  " << ypos << std::endl;
				if (leftButtonPressed && dx < xpos)
				{
					mainnode.GetTransform()->RotY(0.01f);
				}
				else if (leftButtonPressed && dx > xpos)
				{
					mainnode.GetTransform()->RotY(-0.01f);
				}
				else if (leftButtonPressed && dy < ypos)
				{
					mainnode.GetTransform()->RotX(0.01f);
				}
				if (leftButtonPressed && dy > ypos)
				{
					mainnode.GetTransform()->RotX(-0.01f);
				}
				dx = xpos;
				dy = ypos;
			} 
			
		);
		


		
		if (this->window->Open())
		{
			glClearColor(1.0f, 0.5f, 1.0f, 1.0f);
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);

			int w, h;
			
			this->window->GetSize(w, h);
			r = Renderer(w, h);





			glGenTextures(1, &tex_h);
						
			r.LoadOBJModel("./resources/suz.obj");
			r.Draw(1);
			glBindTexture(GL_TEXTURE_2D, tex_h);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,w,h,0,GL_RGBA,GL_UNSIGNED_BYTE,r.GetFramebuffer());
			glBindTexture(GL_TEXTURE_2D, 0);


			return true;
		}
		
		return false;
	}

	//------------------------------------------------------------------------------
	/**
	*/

	void
	Lab5::Run()
	{
		std::vector<Vertex> quadV = { Vertex(vec3(1.0f, 1.0f, 0.0f), vec3(1.0f,1.0f,0), vec3()), // top r
									  Vertex(vec3(1.0f, -1.0f,  0.0f), vec3(1.0f,0.0f,0), vec3()),  // botom r
					  				  Vertex(vec3(-1.0f, -1.0f, 0.0f), vec3(0.0f,0.0f,0), vec3()),// bot l
									  Vertex(vec3(-1.0f, 1.0f, 0.0f), vec3(0.0f,1.0f,0), vec3()) };//top l

		std::vector<GLuint> quadI = { 0,1,3,
									  1,2,3};

		ShaderResource quadShader("./resources/quad.glsl");

		MeshResource m(quadV, quadI);
		m.SetupMeshResource();
		//r.LoadOBJModel("./resources/suz.obj");
		//r.AddBuffer(qube,indices, 0);
		r.SaveFB();
		while (this->window->IsOpen())
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			quadShader.Bind();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, tex_h);
			quadShader.SetUniformTex("tex", 0);
			m.BindVao();
			m.BindIbo();
//			glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,w,h,0,GL_RGBA,GL_UNSIGNED_BYTE,r.GetFramebuffer());
			//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, r.GetWidth(), r.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, r.GetFramebuffer());
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
			glBindTexture(GL_TEXTURE_2D, 0);
			this->window->Update();
			this->window->SwapBuffers();
		}
	}
} // namespace Example
