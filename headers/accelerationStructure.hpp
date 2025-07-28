#ifndef CRTACCELERATIONSTRUCTURE
#define CRTACCELERATIONSTRUCTURE

#include "AABB.hpp"
#include "intersection.hpp"
#include "mesh.hpp"
#include "object.hpp"
#include "ray.hpp"
#include "scene.hpp"
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
        AccelerationStructure(CRTScene* scene);
        bool findIntersection(const CRTRay& ray, Intersection& isect, const float maxT=std::numeric_limits<float>::max());
        void rebuild();
    private:
        std::vector<CRTTriangle> triangleSoup;
        std::vector<ASNode> accTree;
        CRTScene* scene;

        const bool AABBTriIntersection(const AABB& aabb, const CRTTriangle& tri);
        void AABBSplitting(const AABB& toSplit, AABB& a, AABB& b, int axis); //axis: 0 = x; 1=y; 2= z;
        void buildAccTree(int parentIdx, int depth, std::vector<int> triangleSoupIndexes);
        void createTriangleSoup(std::vector<CRTObject> objects);
        void createTriangleSoup();
        void clear();
        void buildKDTree();
};

#endif