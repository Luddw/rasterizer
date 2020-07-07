#include "cam.h"



Cam::Cam(Vector4D startpos, Vector4D target) : pos(startpos)
{
	
	LookAt(target, Vector4D(0, 1));
	SetPerspective(0, 0, 0, 0);
}

Cam::~Cam()
{
}

Cam::Cam(Vector4D target)
{
	LookAt(target, Vector4D(0, 1));
	SetPerspective(0,0,0,0);
}

void Cam::LookAt(Vector4D target, Vector4D up)
{
	Vector4D z_axis = (pos - target);
	z_axis = z_axis.Normalize();
	Vector4D x_axis = z_axis.CrossProduct(up);
	x_axis = x_axis.Normalize();
	Vector4D y_axis = x_axis.CrossProduct(z_axis);
	const Matrix4D viewmat(
		x_axis.GetX(), x_axis.GetY(), x_axis.GetZ(), -1*x_axis.DotProduct(pos),
		y_axis.GetX(), y_axis.GetY(), y_axis.GetZ(), -1*y_axis.DotProduct(pos),
		z_axis.GetX(), z_axis.GetY(), z_axis.GetZ(), -1*z_axis.DotProduct(pos),
		0, 0, 0, 1
	);
	
	view = viewmat;
}

void Cam::SetPerspective(float fov, float aspectratio, float near, float far)
{
	perspective = Matrix4D::PerspProj(1.5707f, (static_cast<float>(4) / static_cast<float>(3)), 0.1f, 100.0f);
}

Matrix4D Cam::GetView() const
{
	return view;
}

Vector4D Cam::GetPos() const
{
	return pos;
}

Matrix4D Cam::GetProj() const
{
	return perspective;
}

