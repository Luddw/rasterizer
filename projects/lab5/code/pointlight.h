#pragma once
#include "matlib.h"
#include "graphicnode.h"

class PointLight 
{
public:
	PointLight();
	PointLight(Vector4D pos, Vector4D color, float intensity, float ambient);
	Vector4D GetPos() const;
	Vector4D GetColor() const;
	float GetAmbient() const;
	float GetIntensity() const;
	void Rotate(float value);
private:
	Vector4D pos;
	Vector4D color;
	float ambient;
	float intensity;
};