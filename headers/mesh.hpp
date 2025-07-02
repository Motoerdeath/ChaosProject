#ifndef CRTMESH
#define CRTMESH

#include "crtVector.hpp"
#include "triangle.hpp"
#include <vector>
class CRTMesh {
    public:
        CRTMesh();
        CRTMesh(std::vector<CRTVector> vertices,std::vector<int> indices) : triangleSoup(vertices), triangleVertIndices(indices) {}
        std::vector<CRTVector> triangleSoup;
        std::vector<int> triangleVertIndices;
    private:


};
#endif