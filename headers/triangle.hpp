#ifndef CRTTRIANGLE
#define CRTTRIANGLE

#include "../headers/crtVector.hpp"
#include <vector>

class CRTTriangle {
    public:
        CRTTriangle() : v0(CRTVector(0.f)), v1(CRTVector(0.f)), v2(CRTVector(0.f)), normal((v1-v0).cross(v2-v0).normalize()), color({0,0,0}) { normal = calculateTriangleNormal();}
        CRTTriangle(CRTVector vall) : v0(vall), v1(vall), v2(vall), normal(CRTVector(0.0f)), color({0,0,0}) {normal = calculateTriangleNormal();}
        CRTTriangle(CRTVector v0,CRTVector v1,CRTVector v2) : v0(v0), v1(v1), v2(v2), normal(CRTVector(0.0f)), color({0,0,0}) {normal = calculateTriangleNormal();}
        CRTVector calculateBarycentricCoordinates(CRTTriangle triangle, CRTVector position);
        CRTVector calculateTriangleNormal() {
            return (v1-v0).cross(v2-v0).normalize();
        }
        CRTVector v0;
        CRTVector v1;
        CRTVector v2;
        CRTVector normal;
        std::vector<int> color;
        CRTVector color2 = CRTVector(0.f);
    private:


};


#endif