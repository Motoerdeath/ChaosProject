#ifndef CRTACCELERATIONSTRUCTURE
#define CRTACCELERATIONSTRUCTURE

#include "AABB.hpp"
#include "intersection.hpp"
#include "triangle.hpp"
#include <vector>

struct ASNode{
    int parentIDx=-1;
    int child1=-1;
    int child2=-1;
    AABB boundingBox;
    std::vector<CRTTriangle> triangles;

};
class AccelerationStructure{
    public:
        AccelerationStructure() {}
        AccelerationStructure createAccelerationStructure();
        Intersection findIntersection();
    private:
        std::vector<CRTTriangle> triangleSoup;
        std::vector<ASNode> structure;
        int MAXTRIANGLESPERLEAF;

};

class kdTree : public AccelerationStructure{

};

class BVHTree : public AccelerationStructure{

};

#endif