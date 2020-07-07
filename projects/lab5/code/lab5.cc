//------------------------------------------------------------------------------
// Lab5.cc
// (C) 2015-2018 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "lab5.h"
#include <sstream>
#include <GL/glew.h>
#include <iostream>
#include <fstream>

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
			glClearColor(0, 0, 0, 1.0f);
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
			Matrix4D m;
			Matrix4D m123(1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
			Matrix4D rot;
			Matrix4D rot2;
			Vector4D startcam(0, 0.8f, 2, 1);
			const auto mesh = std::make_shared<MeshResource>();
			mesh->ObjLoad("./resources/suzSmooth.obj");
			//mesh->ObjLoad("./cube.obj");
			//mesh->ObjLoad("./resources/floor.obj");

			const auto tex = std::make_shared<Texture>("./resources/wood.jpg");
			const auto shad = std::make_shared<ShaderResource>("./resources/shader.glsl");
			const auto tran = std::make_shared<Transform>(m, m, m);
			const auto cam = std::make_shared<Cam>(startcam, Vector4D(0, 0, 0, 1));
			mainnode = GraphicNode(mesh, tex, shad, tran, cam);
			light = PointLight(Vector4D(1, 1, 0.25), Vector4D(1, 1, 1, 1), 0.3, 0.3);
			
			//mesh->DrawCube(0.5f);
			//mesh->DrawCube(0.5);
			
			mainnode.SetLight(light);

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
		float rotlight = 0.05f;
		while (this->window->IsOpen())
		{
			light.Rotate(rotlight);
			//rotlight += 0.05f;
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			mainnode.Draw();
			//std::cout << light.GetPos().GetX() << "\n";
			this->window->Update();
			this->window->SwapBuffers();
		}
	}
} // namespace Example
