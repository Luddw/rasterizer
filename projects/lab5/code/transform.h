#pragma once
#include "matlib.h"

class Vector4D;
class Matrix4D;

class Transform
{
private:
	Matrix4D translation;
	Matrix4D scale;
	Matrix4D rot;

	Matrix4D model;	
public:
	Transform(Matrix4D translate, Matrix4D scale, Matrix4D rotate);
	~Transform();

	void RotX(float rad);
	void RotY(float rad);
	void RotZ(float rad);
	void RotVect(Vector4D vect, float rad);
	void Move(float x,float y,float z);
	void Scale(float x, float y, float z);

	Matrix4D MakeModel();

};

