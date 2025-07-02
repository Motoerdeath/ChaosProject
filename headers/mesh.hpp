#ifndef CRTMESH
#define CRTMESH

#include "crtVector.hpp"
#include "triangle.hpp"
#include <vector>
class CRTMesh {
    public:
        std::vector<CRTVector> triangleSoup;
        std::vector<int> triangleVertIndices;
    private:


};
#endif