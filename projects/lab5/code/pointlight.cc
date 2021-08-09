#include "pointlight.h"

PointLight::PointLight() {
	
	this->intensity = 1;
	this->ambient = 1;
}
PointLight::PointLight(Vector4D pos, Vector4D color, float intensity, float ambient)
{
	this->pos = pos;
	this->color = color;
	this->intensity = intensity;
	this->ambient = ambient;
}

Vector4D PointLight::GetPos() const
{
	return pos;
}

Vector4D PointLight::GetColor() const
{
	return color;
}

float PointLight::GetAmbient() const
{
	return ambient;
}

float PointLight::GetIntensity() const
{
	return intensity;
}

void PointLight::Rotate(float value) 
{
	pos = Matrix4D::RotY_axis(value) * pos;
	
}

