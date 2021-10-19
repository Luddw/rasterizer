#pragma once
#include <cmath>
#include <assert.h>

struct vec4
{
    union {
        struct
        {
            float x;
            float y;
            float z;
            float w;
        };
        float v[4];
    };

    vec4() = default;
    vec4(float const x, float const y, float const z, float const w) : x(x), y(y), z(z), w(w)
    {
        // empty
    }


    vec4 operator+(vec4 const& rhs) const { return {x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w}; }
    void operator+=(vec4 const& rhs) { x += rhs.x; y += rhs.y; z += rhs.z; w += rhs.w; }
    vec4 operator-(vec4 const& rhs) const { return {x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w}; }
    void operator-=(vec4 const& rhs) { x -= rhs.x; y -= rhs.y; z -= rhs.z; w -= rhs.w; }
    vec4 operator-() const { return {-x, -y, -z, -w}; }
    vec4 operator*(float const c) const { return {x * c, y * c, z * c, w * c}; }
    vec4 operator*(float c) { return {x * c, y * c, z * c, w * c}; }
    void operator*=(float const c) { x *= c; y *= c; z *= c; w *= c; }
    void operator/=(float const c) { x /= c; y /= c; z /= c; w /= c; }
    bool operator==(vec4 const& rhs) { return (x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w); }
    bool operator!=(vec4 const& rhs) { return (x != rhs.x && y != rhs.y && z != rhs.z && w != rhs.w); }
    float& operator[](unsigned int i) { assert(i >= 0 && i < 4); return v[i]; }
    vec4 interpolate(const vec4 target, float alpha) const
    {
        return *this + (target - *this) * alpha;
    }
};

// Get length of 3D vector
inline float length(vec4 const v)
{
    float a = v.x * v.x;
    a = a + v.y * v.y;
    a = a + v.z * v.z;
    a = a + v.w * v.w;
    float l = sqrt(a);
    return l;
}

// Get normalized version of v
inline vec4 normalize(vec4 const v)
{
    float l = length(v);
    if (l == 0)
        return vec4(0.0f,0.0f,0.0f,0.0f);

    float div = 1.0f / l;

    vec4 ret = vec4(v.x * div, v.y * div, v.z * div, v.w * div);
    return vec4(ret);
}

// piecewise multiplication between two vectors
inline vec4 mul(vec4 const a, vec4 const b)
{
    return {a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w};
}

// piecewise add between two vectors
inline vec4 add(vec4 const a, vec4 const b)
{
    return {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
}

inline float dot(vec4 const a, vec4 const b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

inline vec4 reflect(vec4 const v, vec4 const n)
{
    return v - n * (2 * dot(v,n));
}


