#pragma once
#include <cmath>
#include <assert.h>
#include "vec4.h"
struct vec3
{
    union {
        struct
        {
            float x;
            float y;
            float z;
        };
        float v[3];
    };
    
    vec3() = default;
    vec3(float const x, float const y, float const z) : x(x), y(y), z(z)
    {
        // empty
    }
    vec3(float const x, float const y) : x(x), y(y), z(0) {};

    vec3(vec4 v0) : x(v0.x), y(v0.y), z(v0.z) {};

    vec3 operator+(vec3 const& rhs) const { return {x + rhs.x, y + rhs.y, z + rhs.z}; }
    void operator+=(vec3 const& rhs) { x += rhs.x; y += rhs.y; z += rhs.z; }
    vec3 operator-(vec3 const& rhs) const { return {x - rhs.x, y - rhs.y, z - rhs.z}; }
    void operator-=(vec3 const& rhs) { x -= rhs.x; y -= rhs.y; z -= rhs.z; }
    vec3 operator-() const { return {-x, -y, -z}; }
    vec3 operator*(float const c) const { return {x * c, y * c, z * c}; }
    vec3 operator/(float const c) const { return {x / c, y / c, z / c}; }
    void operator*=(float const c) { x *= c; y *= c; z *= c; }
    bool operator==(vec3 const& rhs) { return (x == rhs.x && y == rhs.y && z == rhs.z); }
    bool operator==(vec3 const& rhs) const { return (x == rhs.x && y == rhs.y && z == rhs.z); }
    
    bool operator!=(vec3 const& rhs) { return (x != rhs.x && y != rhs.y && z != rhs.z); }
    bool operator!=(vec3 const& rhs) const { return (x != rhs.x && y != rhs.y && z != rhs.z); }
    float& operator[](unsigned int i) { assert(i >= 0 && i < 3); return v[i]; }

    vec3 interpolate(const vec3 target, float alpha) const 
    {
        return *this + (target - *this) * alpha;
    }
};

// Get length of 3D vector
inline float length(vec3 const v)
{
    float a = v.x * v.x;
    a = a + v.y * v.y;
    a = a + v.z * v.z;
    float l = sqrt(a);
    return l;
}

// Get normalized version of v
inline vec3 normalize(vec3 const v)
{
    float l = length(v);
    if (l == 0)
        return vec3(0,0,0);

    vec3 ret = vec3(v.x / l, v.y / l, v.z / l);
    return vec3(ret);
}

// piecewise multiplication between two vectors
inline vec3 mul(vec3 const a, vec3 const b)
{
    return {a.x * b.x, a.y * b.y, a.z * b.z};
}

// piecewise add between two vectors
inline vec3 add(vec3 const a, vec3 const b)
{
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}

inline float dot(vec3 const a, vec3 const b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline vec3 reflect(vec3 const v, vec3 const n)
{
    return v - n * (2 * dot(v,n));
}

inline vec3 cross(vec3 const a, vec3 const b)
{
    return { a.y * b.z - a.z * b.y,
             a.z * b.x - a.x * b.z,
             a.x * b.y - a.y * b.x, };
}

inline vec3 max(vec3 const a, vec3 const b)
{
    vec3 newvect;

    if (a.x >= b.x)
        newvect.x = a.x;
    else
        newvect.x = b.x;
    
    if (a.y >= b.y)
        newvect.y = a.y;
    else
        newvect.y = b.y;
    
    if (a.z >= b.z)
        newvect.z = a.z;
    else
        newvect.z = b.z;
    
    return newvect;
}
inline vec3 min(vec3 const a, vec3 const b)
{
    vec3 newvect;

    if (a.x <= b.x)
        newvect.x = a.x;
    else
        newvect.x = b.x;
    
    if (a.y <= b.y)
        newvect.y = a.y;
    else
        newvect.y = b.y;
    
    if (a.z <= b.z)
        newvect.z = a.z;
    else
        newvect.z = b.z;
    
    return newvect;
}





