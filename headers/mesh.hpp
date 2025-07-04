#ifndef CRTMESH
#define CRTMESH

#include "crtVector.hpp"
#include "triangle.hpp"
#include <vector>
class CRTMesh {
    public:
        CRTMesh();
        CRTMesh(std::vector<CRTVector> vertices,std::vector<int> indices) : triangleVertices(vertices), triangleVertIndices(indices) { vertexNormals.resize(triangleVertices.size());determineVertexNormals();}
        std::vector<CRTVector> triangleVertices;
        std::vector<int> triangleVertIndices;
        std::vector<CRTVector> vertexNormals;
    private:
        void determineVertexNormals();


};
#endif