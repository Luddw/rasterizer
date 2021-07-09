#ifndef __MODEL_H__
#define __MODEL_H__
#include <vector>
#include <string>
#include "mat4.h"

class Model {
private:
    std::vector<vec3> verts_;     // array of vertices
    std::vector<int> facet_vrt_; 
public:
    Model(const std::string filename);
    int nverts() const;
    int nfaces() const;
    vec3 vert(const int i) const;
    vec3 vert(const int iface, const int nthvert) const;
};
#endif //__MODEL_H__
