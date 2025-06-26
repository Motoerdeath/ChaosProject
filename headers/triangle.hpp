#ifndef CRTTRIANGLE
#define CRTTRIANGLE

#include "../headers/crtVector.hpp"

class CRTTriangle {
    public:
        CRTTriangle() : v0(CRTVector(0.f)), v1(CRTVector(0.f)), v2(CRTVector(0.f)) {}
        CRTTriangle(CRTVector vall) : v0(vall), v1(vall), v2(vall) {}
        CRTTriangle(CRTVector v0,CRTVector v1,CRTVector v2) : v0(v0), v1(v1), v2(v2) {}
        CRTVector calculateBarycentricCoordinates(CRTTriangle triangle, CRTVector position);
        CRTVector calculateTriangleNormal() {
            return (v1-v0).cross(v2-v0).normalize();
        }
        CRTVector v0;
        CRTVector v1;
        CRTVector v2;
    private:


};


#endif