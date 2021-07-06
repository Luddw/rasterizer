#pragma once
#include "mlib.h"
#include "vec4.h"
#include "vec3.h"

//------------------------------------------------------------------------------
/**
    @struct mat4

    4x4 matrix
*/
struct mat4
{
    union {
        struct {
            float m00, m01, m02, m03;
            float m10, m11, m12, m13;
            float m20, m21, m22, m23;
            float m30, m31, m32, m33;
        };
        float m[16];
        vec4 v[4];
    };

    mat4() : m00(1), m01(0), m02(0), m03(0),
             m10(0), m11(1), m12(0), m13(0),
             m20(0), m21(0), m22(1), m23(0),
             m30(0), m31(0), m32(0), m33(1)
    {
        // empty
    }
    mat4(vec4 const r0, vec4 const r1, vec4 const r2, vec4 const r3) : v{r0, r1, r2 ,r3}
    {
        // empty
    }
    mat4(float const m00, float const m01, float const m02, float const m03,
         float const m10, float const m11, float const m12, float const m13,
         float const m20, float const m21, float const m22, float const m23,
         float const m30, float const m31, float const m32, float const m33) :
         m00(m00), m01(m01), m02(m02), m03(m03),
         m10(m10), m11(m11), m12(m12), m13(m13),
         m20(m20), m21(m21), m22(m22), m23(m23),
         m30(m30), m31(m31), m32(m32), m33(m33)
    {
        // empty
    }

    bool operator==(mat4 const& rhs) {return v[0] == rhs.v[0] && v[1] == rhs.v[1] && v[2] == rhs.v[2] && v[3] == rhs.v[3];}
    bool operator!=(mat4 const& rhs) {return v[0] != rhs.v[0] && v[1] != rhs.v[1] && v[2] != rhs.v[2] && v[3] != rhs.v[3];}
    mat4 operator*(mat4 const& rhs) const;
    vec4 operator*(vec4 const& v) const;
    vec4& operator[](unsigned int i) {assert(i >= 0 && i < 4); return this->v[i];}
};

//------------------------------------------------------------------------------
/**
*/
inline vec4
get_row0(mat4 m)
{
    return {m.m00, m.m01, m.m02, m.m03};
}

//------------------------------------------------------------------------------
/**
*/
inline vec4
get_row1(mat4 m)
{
    return { m.m10, m.m11, m.m12, m.m13 };
}

//------------------------------------------------------------------------------
/**
*/
inline vec4
get_row2(mat4 m)
{
    return { m.m20, m.m21, m.m22, m.m23 };
}

//------------------------------------------------------------------------------
/**
*/
inline vec4
get_row3(mat4 m)
{
    return { m.m30, m.m31, m.m32, m.m33 };
}

//------------------------------------------------------------------------------
/**
*/
inline vec3
get_position(mat4 m)
{
    return { m.m30, m.m31, m.m32 };
}

//------------------------------------------------------------------------------
/**
    transform vector with matrix basis
*/
inline vec3
transform(vec3 v, mat4 m)
{
    //swizzle!
    //this should be easy to vectorize! ;)
    vec3 x = {v.x, v.x, v.x};
    vec3 y = {v.y, v.y, v.y};
    vec3 z = {v.z, v.z, v.z};
    vec3 r0 = { m.m00, m.m01, m.m02 };
    vec3 r1 = { m.m10, m.m11, m.m12 };
    vec3 r2 = { m.m20, m.m21, m.m22 };
    // multiply
    vec3 a = mul(x, r0);
    vec3 b = mul(y, r1);
    vec3 c = mul(z, r2);
    // add
    vec3 res = add(a,b);
    res = add(res,c);
    return res;
}

//------------------------------------------------------------------------------
/**
    transform vector with matrix basis
*/
inline vec4
transform(vec4 v, mat4 m)
{
    //swizzle!
    vec4 x = {v.x, v.x, v.x, v.x};
    vec4 y = {v.y, v.y, v.y, v.y};
    vec4 z = {v.z, v.z, v.z, v.z};
    vec4 w = {v.w, v.w, v.w, v.w};
    vec4 r0 = { m.m00, m.m01, m.m02, m.m03 };
    vec4 r1 = { m.m10, m.m11, m.m12, m.m13 };
    vec4 r2 = { m.m20, m.m21, m.m22, m.m23 };
    vec4 r3 = { m.m30, m.m31, m.m32, m.m33 };
    // multiply
    vec4 a = mul(x, r0);
    vec4 b = mul(y, r1);
    vec4 c = mul(z, r2);
    vec4 d = mul(w, r3);
    // add
    vec4 res = add(a,b);
    res = add(res,c);
    res = add(res,d);
    return res;
}

//------------------------------------------------------------------------------
/**
    Create a cartesian space transform from a single normal
*/
inline mat4
TBN(vec3 normal)
{
    mat4 ret;
    ret.m10 = normal.x;
    ret.m11 = normal.y;
    ret.m12 = normal.z;
    ret.m13 = 0.0f;

    if (normal.z < -0.999805696f)
    {
        ret.m00 = 0.0f;
        ret.m01 = -1.0f;
        ret.m02 = 0.0f;
        ret.m03 = 0.0f;

        ret.m20 = -1.0f;
        ret.m21 = 0.0f;
        ret.m22 = 0.0f;
        ret.m23 = 0.0f;
    }
    else
    {
        float a = 1.0f / (1.0f + normal.z);
        float b = -normal.x * normal.y * a;

        ret.m00 = 1.0f - normal.x * normal.x * a;
        ret.m01 = b;
        ret.m02 = -normal.x;
        ret.m03 = 0.0f;

        ret.m20 = b;
        ret.m21 = 1.0f - normal.y * normal.y * a;
        ret.m22 = normal.y;
        ret.m23 = 0.0f;
    }

    ret.m30 = 0.0f;
    ret.m31 = 0.0f;
    ret.m32 = 0.0f;
    ret.m33 = 0.0f;
    return ret;
}

//------------------------------------------------------------------------------
/**
    Calculate determinant
*/
inline float
determinant(mat4 m)
{
    return 
        (m.m00 * m.m11 - m.m01 * m.m10) * (m.m22 * m.m33 - m.m23 * m.m32)
       -(m.m00 * m.m12 - m.m02 * m.m10) * (m.m21 * m.m33 - m.m23 * m.m31)
       +(m.m00 * m.m13 - m.m03 * m.m10) * (m.m21 * m.m32 - m.m22 * m.m31)
       +(m.m01 * m.m12 - m.m02 * m.m11) * (m.m20 * m.m33 - m.m23 * m.m30)
       -(m.m01 * m.m13 - m.m03 * m.m11) * (m.m20 * m.m32 - m.m22 * m.m30)
       +(m.m02 * m.m13 - m.m03 * m.m12) * (m.m20 * m.m31 - m.m21 * m.m30);
}

//------------------------------------------------------------------------------
/**
    Calculate inverse of matrix
*/
inline mat4
inverse(mat4 m)
{
    float s = determinant(m);
    
    if (s == 0.0) 
		return {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1}; // cannot inverse, make it identity matrix

    s = 1.0f / s;

    return { s * (m.m11*(m.m22*m.m33 - m.m23*m.m32) + m.m12*(m.m23*m.m31 - m.m21*m.m33) + m.m13*(m.m21*m.m32 - m.m22*m.m31)),
             s * (m.m21*(m.m02*m.m33 - m.m03*m.m32) + m.m22*(m.m03*m.m31 - m.m01*m.m33) + m.m23*(m.m01*m.m32 - m.m02*m.m31)),
             s * (m.m31*(m.m02*m.m13 - m.m03*m.m12) + m.m32*(m.m03*m.m11 - m.m01*m.m13) + m.m33*(m.m01*m.m12 - m.m02*m.m11)),
             s * (m.m01*(m.m13*m.m22 - m.m12*m.m23) + m.m02*(m.m11*m.m23 - m.m13*m.m21) + m.m03*(m.m12*m.m21 - m.m11*m.m22)),
             s * (m.m12*(m.m20*m.m33 - m.m23*m.m30) + m.m13*(m.m22*m.m30 - m.m20*m.m32) + m.m10*(m.m23*m.m32 - m.m22*m.m33)),
             s * (m.m22*(m.m00*m.m33 - m.m03*m.m30) + m.m23*(m.m02*m.m30 - m.m00*m.m32) + m.m20*(m.m03*m.m32 - m.m02*m.m33)),
             s * (m.m32*(m.m00*m.m13 - m.m03*m.m10) + m.m33*(m.m02*m.m10 - m.m00*m.m12) + m.m30*(m.m03*m.m12 - m.m02*m.m13)),
             s * (m.m02*(m.m13*m.m20 - m.m10*m.m23) + m.m03*(m.m10*m.m22 - m.m12*m.m20) + m.m00*(m.m12*m.m23 - m.m13*m.m22)),
             s * (m.m13*(m.m20*m.m31 - m.m21*m.m30) + m.m10*(m.m21*m.m33 - m.m23*m.m31) + m.m11*(m.m23*m.m30 - m.m20*m.m33)),
             s * (m.m23*(m.m00*m.m31 - m.m01*m.m30) + m.m20*(m.m01*m.m33 - m.m03*m.m31) + m.m21*(m.m03*m.m30 - m.m00*m.m33)),
             s * (m.m33*(m.m00*m.m11 - m.m01*m.m10) + m.m30*(m.m01*m.m13 - m.m03*m.m11) + m.m31*(m.m03*m.m10 - m.m00*m.m13)),
             s * (m.m03*(m.m11*m.m20 - m.m10*m.m21) + m.m00*(m.m13*m.m21 - m.m11*m.m23) + m.m01*(m.m10*m.m23 - m.m13*m.m20)),
             s * (m.m10*(m.m22*m.m31 - m.m21*m.m32) + m.m11*(m.m20*m.m32 - m.m22*m.m30) + m.m12*(m.m21*m.m30 - m.m20*m.m31)),
             s * (m.m20*(m.m02*m.m31 - m.m01*m.m32) + m.m21*(m.m00*m.m32 - m.m02*m.m30) + m.m22*(m.m01*m.m30 - m.m00*m.m31)),
             s * (m.m30*(m.m02*m.m11 - m.m01*m.m12) + m.m31*(m.m00*m.m12 - m.m02*m.m10) + m.m32*(m.m01*m.m10 - m.m00*m.m11)),
             s * (m.m00*(m.m11*m.m22 - m.m12*m.m21) + m.m01*(m.m12*m.m20 - m.m10*m.m22) + m.m02*(m.m10*m.m21 - m.m11*m.m20)) };
}

//------------------------------------------------------------------------------
/**
    Flip it!
*/
inline mat4
transpose(mat4 m)
{
    return {
        m.m00, m.m10, m.m20, m.m30,
        m.m01, m.m11, m.m21, m.m31,
        m.m02, m.m12, m.m22, m.m32,
        m.m03, m.m13, m.m23, m.m33
    };
}

//------------------------------------------------------------------------------
/**
*/
inline mat4
multiply(mat4 a, mat4 b)
{
    return { b.m00*a.m00 + b.m10*a.m01 + b.m20*a.m02 + b.m30*a.m03,
             b.m01*a.m00 + b.m11*a.m01 + b.m21*a.m02 + b.m31*a.m03,
             b.m02*a.m00 + b.m12*a.m01 + b.m22*a.m02 + b.m32*a.m03,
             b.m03*a.m00 + b.m13*a.m01 + b.m23*a.m02 + b.m33*a.m03,

			 b.m00*a.m10 + b.m10*a.m11 + b.m20*a.m12 + b.m30*a.m13,
             b.m01*a.m10 + b.m11*a.m11 + b.m21*a.m12 + b.m31*a.m13,
             b.m02*a.m10 + b.m12*a.m11 + b.m22*a.m12 + b.m32*a.m13,
             b.m03*a.m10 + b.m13*a.m11 + b.m23*a.m12 + b.m33*a.m13,

			 b.m00*a.m20 + b.m10*a.m21 + b.m20*a.m22 + b.m30*a.m23,
             b.m01*a.m20 + b.m11*a.m21 + b.m21*a.m22 + b.m31*a.m23,
             b.m02*a.m20 + b.m12*a.m21 + b.m22*a.m22 + b.m32*a.m23,
             b.m03*a.m20 + b.m13*a.m21 + b.m23*a.m22 + b.m33*a.m23,

			 b.m00*a.m30 + b.m10*a.m31 + b.m20*a.m32 + b.m30*a.m33,
             b.m01*a.m30 + b.m11*a.m31 + b.m21*a.m32 + b.m31*a.m33,
             b.m02*a.m30 + b.m12*a.m31 + b.m22*a.m32 + b.m32*a.m33,
             b.m03*a.m30 + b.m13*a.m31 + b.m23*a.m32 + b.m33*a.m33 };
}

//------------------------------------------------------------------------------
/**
*/
inline mat4
rotationx(float rad)
{
	float c;
	float s;

	if (rad == pi)
    {
		s = 0;
	}
	else
    {
		s = sin(rad);
	}

	c = cos(rad);

	return { 1,  0,  0,  0,
             0,  c,  s,  0,
             0,  -s, c,  0,
             0,  0,  0,  1 };
}

//------------------------------------------------------------------------------
/**
*/
inline mat4
rotationy(float rad)
{
	float c;
	float s;

	if (rad == pi)
    {
		s = 0;
	}
	else
    {
		s = sin(rad);
	}

	c = cos(rad);

	return {  c, 0, -s, 0,
			  0, 1,  0, 0,
			  s, 0,  c, 0,
              0, 0,  0, 1 };
}

//------------------------------------------------------------------------------
/**
*/
inline mat4
rotationz(float rad)
{
	float c;
	float s;

	if (rad == pi)
    {
		s = 0;
	}
	else
    {
		s = sin(rad);
	}

	c = cos(rad);

	return {  c,  s,  0,  0,
			 -s,  c,  0,  0,
			  0,  0,  1,  0,
              0,  0,  0,  1 };
}

//------------------------------------------------------------------------------
/**
*/
inline mat4
rotationaxis(vec3 const axis, float rad)
{
	float c;
	float s;

	if (rad == pi)
    {
		s = 0;
	}
	else
    {
		s = sin(rad);
	}

	c = cos(rad);

    float const invC = (1.0f - c);
    vec3 p2 = axis;
    p2 = mul(p2, p2);
    p2 = p2 * invC;
    vec3 c3 = {c,c,c};
    p2 += c3;
    float const xy = axis.x * axis.y;
    float const xz = axis.x * axis.z;
    float const yz = axis.y * axis.z;
    float const zs = axis.z * s;
    float const ys = axis.y * s;
    float const xs = axis.x * s;
    float const cxy = invC * xy;
    float const cyz = invC * yz;
    float const cxz = invC * xz;

    return {  p2.x,      cxy + zs,  cxz - ys,  0,
			  cxy - zs,  p2.y,      cyz + xs,  0,
			  cxz + ys,  cyz - xs,  p2.z,      0,
              0,         0,         0,         1 };
}

inline mat4
perspectiveprojection(float fov, float aspectratio, float near, float far)
{
    const float r = tan(fov/2)*near;
    const float t = r/aspectratio;
    const float l = r*-1;
    const float b = t*-1;
    const float n2 = near*2;
    
    return { n2/(r-l),  0,          (r + l) / (r - l),       0,
             0,         n2/(t-b),   (t + b) / (t - b),       0,
             0,         0,          -((far + near)/(far - near)),   -((2 * far * near) / (far - near)),
             0,         0,          -1,                      0 };
}

inline mat4
lookat(vec3 const & eye, vec3 const & target, vec3 const & upvect)
{
    vec3 z = normalize(target - eye); 
    vec3 x = normalize(cross(z, upvect)); 
    vec3 y = cross(x, z); 
    y = cross(x, z);
    
    mat4 rotation = {x.x, x.y, x.z, 0,
                     y.x, y.y, y.z, 0,
                     -z.x, -z.y, -z.z, 0,
                     0,0,0,1};
    mat4 translation = {1,0,0, -eye.x,
                        0,1,0, -eye.y,
                        0,0,1, -eye.z,
                        0,0,0,1};
    

    return rotation * translation;

    // return {
    //     vec4(x.x, x.y, x.z, -dot(x, eye)),
    //     vec4(y.x, y.y, y.z, -dot(y, eye)),
    //     vec4(-z.x, -z.y, -z.z, -dot(y, eye)),
    //     vec4(0, 0, 0, 1)
    //     }; 
}
inline mat4 mat4::operator*(mat4 const& rhs) const {return multiply(*this, rhs);}
inline vec4 mat4::operator*(vec4 const& v) const {return transform(v, *this);}