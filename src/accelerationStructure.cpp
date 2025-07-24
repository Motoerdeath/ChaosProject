


#include "../headers/accelerationStructure.hpp"
#include <limits>
#include <numeric>
#include <stack>
#include <cassert>



void AccelerationStructure::createTriangleSoup(std::vector<CRTMesh> objects) {
    for(int i = 0; i < objects.size();i++) {
        CRTMesh object = objects[i];
        for(int j = 0; j < object.triangleVertIndices.size();j+=3) {
            CRTTriangle triangle(object.triangleVertices[object.triangleVertIndices[j]],
                            object.triangleVertices[object.triangleVertIndices[j+1]],
                            object.triangleVertices[object.triangleVertIndices[j+2]]);

            triangle.objectID = i;
            triangle.geoNormal = triangle.normal;
            triangle.materialID = object.materialID;
            triangle.vertexNormal0 = object.vertexNormals[object.triangleVertIndices[j]];
            triangle.vertexNormal1 = object.vertexNormals[object.triangleVertIndices[j+1]];
            triangle.vertexNormal2 = object.vertexNormals[object.triangleVertIndices[j+2]];
            if(object.textureCoords.size() == object.triangleVertices.size()) {
                triangle.texCoords0 = object.textureCoords[object.triangleVertIndices[j]];
                triangle.texCoords1 = object.textureCoords[object.triangleVertIndices[j+1]];
                triangle.texCoords2 = object.textureCoords[object.triangleVertIndices[j+2]];
            } else {
                triangle.texCoords0 = CRTVector(0.f);//object.textureCoords[object.triangleVertIndices[j]];
                triangle.texCoords1 = CRTVector(0.f);//object.textureCoords[object.triangleVertIndices[j+1]];
                triangle.texCoords2 = CRTVector(0.f);//object.textureCoords[object.triangleVertIndices[j+2]];
            }

            
            triangleSoup.push_back(triangle);
        }
    }
}
//for he moemnt we only build a kd tree
bool AccelerationStructure::AABBTriIntersection(const AABB& aabb, const CRTTriangle& tri) {
    AABB triaabb{tri};
    CRTVector v1 = triaabb.min-aabb.max;
    CRTVector v2 = aabb.min - triaabb.max;
    if(v1.x > 0.f|| v1.y >0.f || v1.z >0.f || v2.x >0.f || v2.y >0.f || v2.z >0.f) return false;
    return true;
}

bool AccelerationStructure::AABBTriIntersection(const AABB& aabb, const CRTTriangle& tri,int i) {
    AABB triaabb{tri};
    CRTVector v1 = triaabb.min-aabb.max;
    CRTVector v2 = aabb.min - triaabb.max;
    if(v1.x > 0.f|| v1.y >0.f || v1.z >0.f || v2.x >0.f || v2.y >0.f || v2.z >0.f) return false;
    return true;
}

void AccelerationStructure::AABBSplitting(const AABB& toSplit, AABB& a, AABB& b, int axis) {
    float mid = (toSplit.max[axis] - toSplit.min[axis]) /2.f;
    float splitCoordinatePoint = toSplit.min[axis] + mid;
    a = toSplit;
    b = toSplit;
    if(axis == 0) {
        a.max.x = splitCoordinatePoint;
        b.min.x = splitCoordinatePoint;
    }
    if(axis == 1) {
        a.max.y = splitCoordinatePoint;
        b.min.y = splitCoordinatePoint;
    }
    if(axis == 2) {
        a.max.z = splitCoordinatePoint;
        b.min.z = splitCoordinatePoint;
    }
}
void AccelerationStructure::buildAS() {
    ASNode root;
    root.child1 =-1;
    root.child2 =-1;
    root.parentIDx =-1;
    AABB rootBB;
    for(CRTTriangle tri : triangleSoup) {
        rootBB.include(tri);
    }
    root.boundingBox = rootBB;
    accTree.push_back(root);
    std::vector<int> triangleIndexes(triangleSoup.size());
    std::iota(triangleIndexes.begin(), triangleIndexes.end(), 0);

    buildAccTree(0, 0, triangleIndexes);

}

void AccelerationStructure::buildAccTree(int parentIdx, int depth, std::vector<CRTTriangle> triangles) {
    if(triangles.size() <= MAXTRIANGLESPERLEAF || depth >= MAXTREEDEPTH) {
        //build leaf node
        accTree[parentIdx].triangles = triangles;
        return;
    } else {
        int splittingAxis = depth% 3;
        AABB child1AABB;
        AABB child2AABB;
        AABBSplitting(accTree[parentIdx].boundingBox, child1AABB, child2AABB, splittingAxis);
        std::vector<CRTTriangle> child1triangles;
        std::vector<CRTTriangle> child2triangles;
        for(CRTTriangle tri : triangles) {
            if(AABBTriIntersection(child1AABB, tri)) {
                child1triangles.push_back(tri);
            }
            if(AABBTriIntersection(child2AABB, tri)) {
                child2triangles.push_back(tri);
            }
        }
        //if there are triangles in child 1 then build that subnode
        if(child1triangles.size()>0) {  
            ASNode child1;
            child1.child1  = -1;
            child1.child2  = -1;
            child1.boundingBox = child1AABB;
            child1.parentIDx = parentIdx;
            int child1Idx = accTree.size();
            accTree.push_back(child1);
            accTree[parentIdx].child1 =child1Idx;
            buildAccTree(child1Idx, depth+1, child1triangles);
        }
        if(child2triangles.size() > 0) {
            ASNode child2;
            child2.child1  = -1;
            child2.child2  = -1;
            child2.boundingBox = child2AABB;
            child2.parentIDx = parentIdx;
            int child2Idx = accTree.size();
            accTree.push_back(child2);
            accTree[parentIdx].child2 =child2Idx;
            buildAccTree(child2Idx, depth+1, child2triangles);
        }
    }
}
void AccelerationStructure::buildAccTree(int parentIdx, int depth, std::vector<int> triangleSoupIndexes) {
    if(triangleSoupIndexes.size() <= MAXTRIANGLESPERLEAF || depth >= MAXTREEDEPTH) {
        //build leaf node
        accTree[parentIdx].triangleSoupIdx = triangleSoupIndexes;
        return;
    } else {
        int splittingAxis = depth% 3;
        AABB child1AABB;
        AABB child2AABB;
        AABBSplitting(accTree[parentIdx].boundingBox, child1AABB, child2AABB, splittingAxis);
        std::vector<int> child1triangles;
        std::vector<int> child2triangles;
        for(int triIdx : triangleSoupIndexes) {
            
            if(AABBTriIntersection(child1AABB, triangleSoup[triIdx])) {
                child1triangles.push_back(triIdx);
            }
            if(AABBTriIntersection(child2AABB, triangleSoup[triIdx])) {
                child2triangles.push_back(triIdx);
            }
        }
        //if there are triangles in child 1 then build that subnode
        if(child1triangles.size()>0) {  
            ASNode child1;
            child1.child1  = -1;
            child1.child2  = -1;
            child1.boundingBox = child1AABB;
            child1.parentIDx = parentIdx;
            int child1Idx = accTree.size();
            accTree.push_back(child1);
            accTree[parentIdx].child1 =child1Idx;
            buildAccTree(child1Idx, depth+1, child1triangles);
        }
        if(child2triangles.size() > 0) {
            ASNode child2;
            child2.child1  = -1;
            child2.child2  = -1;
            child2.boundingBox = child2AABB;
            child2.parentIDx = parentIdx;
            int child2Idx = accTree.size();
            accTree.push_back(child2);
            accTree[parentIdx].child2 =child2Idx;
            buildAccTree(child2Idx, depth+1, child2triangles);
        }
    }
}

bool AccelerationStructure::findIntersection(const CRTRay& ray, Intersection& isect, const float maxT) {
    int depth = 0;
    int roodIDx = 0; //for now the root of the AS is always at index 0
    std::stack<int> nodeStack;
    nodeStack.push(roodIDx); //the root of the AS is always checked first
    CRTTriangle closestTriangle;
    int triangleIDx;
    bool foundIntersection = false;
    float minT = std::numeric_limits<float>::max();
    while(!nodeStack.empty()) {
        ASNode* currentNode = &accTree[nodeStack.top()];
        nodeStack.pop();
        if(CRTRay::intersectBoundingBox(ray, currentNode->boundingBox)) {
            if(currentNode->triangleSoupIdx.size() > 0) {
                

                for(int i = 0; i < currentNode->triangleSoupIdx.size();i++) {

                    CRTTriangle tri = triangleSoup[currentNode->triangleSoupIdx[i]];
                    float t;
                    bool hitCondition = false;
                    if(ray.type == ShadowRay ||ray.type == RefractionRay ||ray.type == ReflectionRay){
                        hitCondition = CRTRay::intersectTriangle(ray,tri,t, true) && t < minT && t < maxT;
                    } else {
                        hitCondition = CRTRay::intersectTriangle(ray,tri,t, false) && t < minT && t < maxT;
                    }
                    if(hitCondition) {
                            triangleIDx = currentNode->triangleSoupIdx[i];
                            minT = t;
                            closestTriangle = tri;
                            foundIntersection = true;
                    }

                }
            } else {
                if(currentNode->child1 != -1) nodeStack.push(currentNode->child1); 
                if(currentNode->child2 != -1) nodeStack.push(currentNode->child2); 
            }
        }
    }

    if(!foundIntersection) return false;
    isect.intersectionPoint = ray.rayOrigin + ray.rayDirection * minT;
    //offset intersectionPoint

    isect.geomNormal = closestTriangle.geoNormal;
    isect.triangleIDx = triangleIDx;
    isect.materialIDx = closestTriangle.materialID;
    isect.objectIDx = closestTriangle.objectID;
    isect.t = minT;
    isect.baryCoords = CRTTriangle::calculateBarycentricCoordinates(closestTriangle, isect.intersectionPoint);
    isect.shadingNormal = closestTriangle.vertexNormal0 * isect.baryCoords.z + closestTriangle.vertexNormal1 * isect.baryCoords.x + closestTriangle.vertexNormal2 * isect.baryCoords.y;
    isect.textureCoords = closestTriangle.texCoords0 * isect.baryCoords.z + closestTriangle.texCoords1 * isect.baryCoords.x + closestTriangle.texCoords2 * isect.baryCoords.y;


    return true;
}

/*
        isect.intersectionPoint = position;
        isect.baryCoords = baryCoords;
        isect.geomNormal = geoNormal;
        isect.shadingNormal = shadingNormal;
        isect.materialIDx = materialID;
        isect.objectIDx = objectID;
        isect.triangleIDx = triangleID;
        isect.t = closestIntersectionDistance;
        */