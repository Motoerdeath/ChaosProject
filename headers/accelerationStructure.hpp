#ifndef CRTACCELERATIONSTRUCTURE
#define CRTACCELERATIONSTRUCTURE

#include "AABB.hpp"
#include "intersection.hpp"
#include "mesh.hpp"
#include "object.hpp"
#include "ray.hpp"
#include "triangle.hpp"
#include <vector>
#include "../headers/globalSettings.hpp"

struct ASNode{
    int parentIDx=-1;
    int child1=-1;
    int child2=-1;
    AABB boundingBox;
    std::vector<CRTTriangle> triangles;
    std::vector<int> triangleSoupIdx;

};
class AccelerationStructure{
    public:
        AccelerationStructure() {}
        void buildAS();
        bool findIntersection(const CRTRay& ray, Intersection& isect, const float maxT=std::numeric_limits<float>::max());
        void createTriangleSoup(std::vector<CRTMesh> objects);
        void createTriangleSoup(std::vector<CRTObject> objects);
        void clear();
    private:
        std::vector<CRTTriangle> triangleSoup;
        std::vector<ASNode> accTree;
        bool AABBTriIntersection(const AABB& aabb, const CRTTriangle& tri);

        void AABBSplitting(const AABB& toSplit, AABB& a, AABB& b, int axis); //axis: 0 = x; 1=y; 2= z;
        void buildAccTree(int parentIdx, int depth, std::vector<int> triangleSoupIndexes);

};

#endif