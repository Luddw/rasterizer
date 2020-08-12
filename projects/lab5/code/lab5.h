#pragma once
//------------------------------------------------------------------------------
/**
	Application class used for example application.
	
	(C) 2015-2018 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/app.h"
#include "render/window.h"
#include "graphicnode.h"
#include "pointlight.h"
#include "softwarerasterizer.h"

namespace Example
{
class Lab5 : public Core::App
{
public:
	/// constructor
	Lab5();
	/// destructor
	~Lab5();
	void keyEvent(int key, int action, int modifier);
	void MousePos(float xpos, float ypos);

	/// open app
	bool Open();
	/// run app
	void Run();
private:
	bool leftButtonPressed = false;
	bool ismoving = false;
	float dx = 0;
	float dy = 0;
	GraphicNode mainnode;
	PointLight light;
	Display::Window* window;
	GLuint tex_h;
	Renderer r;


};
} // namespace Example