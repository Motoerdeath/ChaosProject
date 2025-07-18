


#include "../headers/accelerationStructure.hpp"
#include <limits>
#include <stack>



void AccelerationStructure::createTriangleSoup(std::vector<CRTMesh> objects) {
    for(CRTMesh object : objects) {
        for(int i = 0; i < object.triangleVertIndices.size();i+=3) {
            CRTTriangle triangle(object.triangleVertices[object.triangleVertIndices[i]],
                            object.triangleVertices[object.triangleVertIndices[i+1]],
                            object.triangleVertices[object.triangleVertIndices[i+2]]);

            triangleSoup.push_back(triangle);
        }
    }
}

void AccelerationStructure::buildAS() {

}

bool AccelerationStructure::findIntersection(const CRTRay& ray, Intersection& isect, const float maxT) {
    int depth = 0;
    int roodIDx = 0; //for now the root of the AS si always at index 0
    std::stack<int> nodeStack;
    nodeStack.push(roodIDx); //the root of the AS is always checked first
    CRTTriangle closestTriangle;
    bool foundIntersection = false;
    while(!nodeStack.empty()) {
        ASNode* currentNode = &accelerationStructure[nodeStack.top()];
        nodeStack.pop();
        if(CRTRay::intersectBoundingBox(ray, currentNode->boundingBox)) {
            if(currentNode->triangles.size() > 0) {
                float minT = std::numeric_limits<float>::max();

                for(CRTTriangle tri : currentNode->triangles) {
                    float t;
                    if(CRTRay::intersectTriangle(ray,tri,t, false)) {
                        if(t <maxT && t < minT) {
                            minT = t;
                            closestTriangle = tri;
                            foundIntersection = true;
                        }
                    }
                }

            } else {
                if(currentNode->child1 != -1) nodeStack.push(currentNode->child1); 
                if(currentNode->child2 != -1) nodeStack.push(currentNode->child2); 
            }
        } else {
            
        }
    }

    if(!foundIntersection) return false;

    //init the intersectionData


    return true;
}