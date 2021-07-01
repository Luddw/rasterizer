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
			Point p1 = {100, 50};
			Point p2 = {300, 100};
			Point p3 = {40, 400};

			
			
			Pixel pix1 = {254, 0, 0, 254};
			r.RasterizeTriangle(p1,p2,p3, pix1);

			Point p4 = {400,600};
			Point p5 = {350,100};
			Point p6 = {600,350};

			Pixel pix = {0,254,50,254};
			r.RasterizeTriangle(p4,p5,p6, pix);

			Point p41 = {650,50};
			Point p51 = {800,25};
			Point p61 = {700,250};

			Pixel pix3 = {0,25,254,254};
			r.RasterizeTriangle(p41,p51,p61, pix3);

			
			Point middle = {-0.5, 0.0};
			Point middle2 = {0.5, 0.0};
			r.DrawLine(middle,middle2, pix);
			r.SaveFB();
			

			
			glGenTextures(1, &tex_h);
			glBindTexture(GL_TEXTURE_2D, tex_h);

			//std::cout<<"width: "<< widht<< "height: " << height << "bpp: "<<bpp<<std::endl;

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

		int size = 1;
		std::vector<Vertex> qube= { 
		// Left
		Vertex(Vector4D(-size, -size, -size),	Vector4D(1.0f, 0.75f)),
		Vertex(Vector4D(-size, -size, size),   Vector4D(0.75f, 0.75f)),
		Vertex(Vector4D(-size, size, size),		Vector4D(0.75f, 0.5f)),
		Vertex(Vector4D(-size, size, -size),	 Vector4D(1.0f, 0.5f)),

		// Front
		Vertex(Vector4D(-size,-size,size),	  Vector4D(0.25f, 0.75f)),
		Vertex(Vector4D(size, -size, size),	   Vector4D(0.0f, 0.75f)),
		Vertex(Vector4D(size, size, size),	    Vector4D(0.0f, 0.5f)),
		Vertex(Vector4D(-size, size, size),	   Vector4D(0.25f, 0.5f)),



		// Back
		Vertex(Vector4D(size, -size, -size), Vector4D(0.75f, 0.75f)),
		Vertex(Vector4D(-size, -size, -size), Vector4D(0.5f, 0.75f)),
		Vertex(Vector4D(-size, size, -size),   Vector4D(0.5f, 0.5f)),
		Vertex(Vector4D(size, size, -size),   Vector4D(0.75f, 0.5f)),



		// Right
		Vertex(Vector4D(size, -size, size),Vector4D(0.5f, 0.75f)),
		Vertex(Vector4D(size, -size, -size),Vector4D(0.25f, 0.75f)),
		Vertex(Vector4D(size, size, -size),Vector4D(0.25f, 0.5f)),  
		Vertex(Vector4D(size, size, size),  Vector4D(0.5f, 0.5f)),

		Vertex(Vector4D(-size, size, size), Vector4D(0.75f, 0.5f)),
		Vertex(Vector4D(size, size, size),  Vector4D(0.5f, 0.5f)),
		Vertex(Vector4D(size, size, -size), Vector4D(0.5f, 0.25f)),
		Vertex(Vector4D(-size, size, -size), Vector4D(0.75f, 0.25f)),

		// Bottom
		Vertex(Vector4D(size, -size, size),  Vector4D(0.75f, 1.0f)),  
		Vertex(Vector4D(-size, -size, size), Vector4D(0.5f, 1.0f)),   
		Vertex(Vector4D(-size, -size, -size),Vector4D(0.5f, 0.75f)),  
		Vertex(Vector4D(size, -size, -size),  Vector4D(0.75f, 0.75f))

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

		std::vector<Vertex> quadV = { Vertex(Vector4D(1.0f, 1.0f, 0.0f), Vector4D(1.0f,0.0f)), // top r
									  Vertex(Vector4D(1.0f, -1.0f,  0.0f), Vector4D(1.0f,1.0f)),  // botom r
					  				  Vertex(Vector4D(-1.0f, -1.0f, 0.0f), Vector4D(0.0f,1.0f) ),// bot l
									  Vertex(Vector4D(-1.0f, 1.0f, 0.0f), Vector4D(0.0f,0.0f)) };//top l

		std::vector<GLuint> quadI = { 0,1,3,
									  1,2,3};

		ShaderResource quadShader("./resources/quad.glsl");
		
		MeshResource m(quadV, quadI);
		m.SetupMeshResource();

		r.AddBuffer(qube,indices, 0);
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
			//r.Draw();
			//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, r.GetWidth(), r.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, r.GetFramebuffer());
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
			glBindTexture(GL_TEXTURE_2D, 0);

			this->window->Update();
			this->window->SwapBuffers();
		}
	}
} // namespace Example
