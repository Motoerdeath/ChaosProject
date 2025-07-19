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
    std::vector<int> triangleSoupIdx;

};
class AccelerationStructure{
    public:
        AccelerationStructure() {}
        void buildAS();
        bool findIntersection(const CRTRay& ray, Intersection& isect, const float maxT=std::numeric_limits<float>::max());
        void createTriangleSoup(std::vector<CRTMesh> objects);
        static bool AABBTriIntersection(const AABB& aabb, const CRTTriangle& tri, int i);
    private:
        std::vector<CRTTriangle> triangleSoup;
        std::vector<ASNode> accTree;
        int MAXTRIANGLESPERLEAF = 4;
        int MAXTREEDEPTH = 5;
        bool AABBTriIntersection(const AABB& aabb, const CRTTriangle& tri);
        
        void AABBSplitting(const AABB& toSplit, AABB& a, AABB& b, int axis); //axis: 0 = x; 1=y; 2= z;
        void buildAccTree(int parentIdx, int depth, std::vector<CRTTriangle> triangles);
        void buildAccTree(int parentIdx, int depth, std::vector<int> triangleSoupIndexes);

};

class kdTree : public AccelerationStructure{

};

class BVHTree : public AccelerationStructure{

};

#endif