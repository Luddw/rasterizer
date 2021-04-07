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

			r.DrawLine(13, 20, 80, 40);
			r.DrawLine(20, 13, 40, 80);	
			r.DrawLine(80, 40, 13, 20);


			
			r.SaveFB();
			

			
			glGenTextures(1, &tex_h);
			glBindTexture(GL_TEXTURE_2D, tex_h);

			//std::cout<<"width: "<< widht<< "height: " << height << "bpp: "<<bpp<<std::endl;
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);

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
		std::vector<Vertex> quadV = { Vertex(Vector4D(-0.5, 0.5), Vector4D()), Vertex(Vector4D(0.5, 0.5), Vector4D()),
					  Vertex(Vector4D(-0.5, -0.5), Vector4D()), Vertex(Vector4D(0.5, -0.5), Vector4D()), };

		std::vector<unsigned int> quadI = { 2,1,3,
										   3,2,4 };

		MeshResource m(quadV, quadI);
		while (this->window->IsOpen())
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glActiveTexture(GL_TEXTURE0 + 1);

			glBindTexture(GL_TEXTURE_2D, 0);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, r.GetWidth(), r.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, r.GetFramebuffer());
			m.DrawCube(1);
				


			glBindTexture(GL_TEXTURE_2D, 0);

			this->window->Update();
			this->window->SwapBuffers();
		}
	}
} // namespace Example
