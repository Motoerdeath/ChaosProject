#ifndef CRTACCELERATIONSTRUCTURE
#define CRTACCELERATIONSTRUCTURE

#include "AABB.hpp"
#include "intersection.hpp"
#include "mesh.hpp"
#include "ray.hpp"
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
        void buildAS();
        bool findIntersection(const CRTRay& ray, Intersection& isect, const float maxT=std::numeric_limits<float>::max());
        void createTriangleSoup(std::vector<CRTMesh> objects);
    private:
        std::vector<CRTTriangle> triangleSoup;
        std::vector<ASNode> accelerationStructure;
        int MAXTRIANGLESPERLEAF;
        int MAXSEARCHDEPTH;

};

class kdTree : public AccelerationStructure{

};

class BVHTree : public AccelerationStructure{

};

#endif