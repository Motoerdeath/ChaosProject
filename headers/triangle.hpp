#ifndef CRTTRIANGLE
#define CRTTRIANGLE

#include "../headers/crtVector.hpp"

struct CRTTriangle {
    CRTVector v1;
    CRTVector v2;
    CRTVector v3;

    CRTTriangle() : v1(CRTVector(0.f)), v2(CRTVector(0.f)), v3(CRTVector(0.f)) {}
    CRTTriangle(CRTVector vall) : v1(vall), v2(vall), v3(vall) {}
    CRTTriangle(CRTVector v1,CRTVector v2,CRTVector v3) : v1(v1), v2(v2), v3(v3) {}
};

#endif