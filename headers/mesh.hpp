#ifndef CRTMESH
#define CRTMESH

#include "crtVector.hpp"
#include "triangle.hpp"
#include <vector>
class CRTMesh {
    public:
        CRTMesh();
        CRTMesh(std::vector<CRTVector> vertices,std::vector<int> indices) : triangleVertices(vertices), triangleVertIndices(indices) { vertexNormals.reserve(triangleVertices.size());determineVertexNormals();}
        CRTMesh(std::vector<CRTVector> vertices,std::vector<int> indices, int mID) : triangleVertices(vertices), triangleVertIndices(indices),materialID(mID) { vertexNormals.reserve(triangleVertices.size());determineVertexNormals();}

        std::vector<CRTVector> triangleVertices;
        std::vector<int> triangleVertIndices;
        std::vector<CRTVector> vertexNormals;
    private:
        void determineVertexNormals();
        int materialID = 0;


};
#endif