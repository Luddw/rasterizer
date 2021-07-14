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
			Pixel p(0xFF,0,0,0xFF);

			int w, h;
			
			this->window->GetSize(w, h);
			r = Renderer(w, h);




			

			Pixel pix = {0,254,50,254};
			Pixel pix1 = {254,0,50,254};

			
			Point middle = {-1, -0.1};
			Point middle2 = {1, 0.0};
			Point yline = {-1, 0.0};
			Point yline2 = {1, 0.0};

			r.DrawLine(middle,middle2, pix1);
			r.DrawLine(yline, yline2, pix);
			
			r.PlacePixel(1,1,pix1);

			
			glGenTextures(1, &tex_h);
			glBindTexture(GL_TEXTURE_2D, tex_h);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,w,h,0,GL_RGBA,GL_UNSIGNED_BYTE,r.GetFramebuffer());
			glBindTexture(GL_TEXTURE_2D, 0);
			
			r.LoadOBJModel("./resources/suz.obj");
			r.Draw(1);

			r.SaveFB();
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

		auto size = 0.5;
		std::vector<Vertex> qube= { 
		// Left
		Vertex(vec3(-size, -size, -size),	vec3(1.0f, 0.75f,0)  , vec3()),
		Vertex(vec3(-size, -size, size),   	vec3(0.75f, 0.75f,0) , vec3()),
		Vertex(vec3(-size, size, size),		vec3(0.75f, 0.5f,0)  , vec3()),
		Vertex(vec3(-size, size, -size),	vec3(1.0f, 0.5f,0)   , vec3()),

		// Front
		Vertex(vec3(-size,-size,size),	  	vec3(0.25f, 0.75f,0), vec3()),
		Vertex(vec3(size, -size, size),	  	vec3(0.0f, 0.75f,0)	, vec3()),
		Vertex(vec3(size, size, size),	    vec3(0.0f, 0.5f,0) 	, vec3()),
		Vertex(vec3(-size, size, size),	   	vec3(0.25f, 0.5f,0)	, vec3()),



		// Back
		Vertex(vec3(size, -size, -size), 	vec3(0.75f, 0.75f,0), vec3()),
		Vertex(vec3(-size, -size, -size), 	vec3(0.5f, 0.75f,0)	, vec3()),
		Vertex(vec3(-size, size, -size),   	vec3(0.5f, 0.5f,0)	, vec3()),
		Vertex(vec3(size, size, -size),   	vec3(0.75f, 0.5f,0)	, vec3()),



		// Right
		Vertex(vec3(size, -size, size),	vec3(0.5f, 0.75f,0)	, vec3()),
		Vertex(vec3(size, -size, -size),vec3(0.25f, 0.75f,0), vec3()),
		Vertex(vec3(size, size, -size),	vec3(0.25f, 0.5f,0)	, vec3()),  
		Vertex(vec3(size, size, size),  vec3(0.5f, 0.5f,0)	, vec3()),

		Vertex(vec3(-size, size, size), 	vec3(0.75f, 0.5f,0)	, vec3()),
		Vertex(vec3(size, size, size), 		vec3(0.5f, 0.5f,0)	, vec3()),
		Vertex(vec3(size, size, -size), 	vec3(0.5f, 0.25f,0)	, vec3()),
		Vertex(vec3(-size, size, -size),	vec3(0.75f, 0.25f,0), vec3()),

		// Bottom
		Vertex(vec3(size, -size, size),  	vec3(0.75f, 1.0f,0)  , vec3()),  
		Vertex(vec3(-size, -size, size),	vec3(0.5f, 1.0f,0)   , vec3()),   
		Vertex(vec3(-size, -size, -size),	vec3(0.5f, 0.75f,0)  , vec3()),  
		Vertex(vec3(size, -size, -size),  	vec3(0.75f, 0.75f,0) , vec3())

		};

		std::vector<unsigned int> 	indices = {
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
		
		while (this->window->IsOpen())
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			quadShader.Bind();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, tex_h);
			quadShader.SetUniformTex("tex", 0);
			//r.Draw(1);	
			r.Draw(1);

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
