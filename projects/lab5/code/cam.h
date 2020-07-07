#pragma once
#include "matlib.h"
class Vector4D;
class Matrix4D;


class Cam
{
private:
    Vector4D pos;
	Matrix4D perspective;
	Matrix4D view;
public:
    Cam(Vector4D startpos, Vector4D target);
    ~Cam();
    Cam(Vector4D target);
    void LookAt(Vector4D target, Vector4D up);
	void SetPerspective(float fov, float aspectratio, float near, float far);
    void Rot(float rad);

    Matrix4D GetView() const;
	Matrix4D GetProj() const;
	Vector4D GetPos() const;

};

