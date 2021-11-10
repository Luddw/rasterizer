#pragma once
#include "vec3.h"
struct Vertex
{
	Vertex() : pos(vec3()), uv(vec3()) { };
	Vertex(const Vertex& other) : pos(other.pos), uv(other.uv) , normal(other.normal) { };
	Vertex(vec3 position, vec3 uv, vec3 norm) : pos(position), uv(uv), normal(norm) { };
	//Vertex(const vec3& position, const vec3& uv/*, const vec3& norm*/): pos(position), uvPos(uv)/*,normal(norm)*/{};
	vec3 pos;
	vec3 uv;
    vec3 normal;
	
};
