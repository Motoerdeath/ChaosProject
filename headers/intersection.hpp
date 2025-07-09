#ifndef CRTINTERSECTION
#define CRTINTERSECTION

#include "crtVector.hpp"
#include "mesh.hpp"
#include "triangle.hpp"
struct Intersection{
    CRTVector intersectionPoint;
    CRTMesh* intersectedObject;
    CRTTriangle intersectionTriangle;
    CRTVector baryCoords;
    CRTVector shadingNormal;
    int mID;
    float t;
    Intersection() {};
};

#endif