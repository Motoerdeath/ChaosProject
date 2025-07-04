#include "../headers/mesh.hpp"
#include <vector>


void CRTMesh::determineVertexNormals() {

    
    for(int vertexIndex = 0; vertexIndex < triangleVertices.size();vertexIndex++) {
        CRTVector vertex = triangleVertices[vertexIndex];
        CRTVector vertexNormal(0.f);
        for(int j = 0; j < triangleVertIndices.size();j++) {
            if(triangleVertIndices[j] == vertexIndex) {
                CRTTriangle triangle;
                if(j % 3 == 0) {
                    triangle = CRTTriangle(triangleVertices[triangleVertIndices[j]],triangleVertices[triangleVertIndices[j+1]],triangleVertices[triangleVertIndices[j+2]]);
                } else if( j % 3 == 1) {
                    triangle = CRTTriangle(triangleVertices[triangleVertIndices[j-1]],triangleVertices[triangleVertIndices[j]],triangleVertices[triangleVertIndices[j+1]]);
                } else {
                    triangle = CRTTriangle(triangleVertices[triangleVertIndices[j-2]],triangleVertices[triangleVertIndices[j-1]],triangleVertices[triangleVertIndices[j]]);
                }
                vertexNormal= vertexNormal+triangle.normal;
            }
        }
        vertexNormals.push_back(vertexNormal.normalize());
    }

    
}