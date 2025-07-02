#ifndef CRTMESH
#define CRTMESH

#include "triangle.hpp"
#include <vector>
class CRTMesh {

    private:
        std::vector<CRTTriangle> triangleSoup;
        std::vector<int> triangleVertIndices;

};
#endif