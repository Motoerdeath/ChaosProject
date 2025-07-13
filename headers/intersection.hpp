#ifndef CRTINTERSECTION
#define CRTINTERSECTION

#include "crtVector.hpp"
#include "mesh.hpp"
#include "triangle.hpp"
struct Intersection{
    CRTVector intersectionPoint;
    CRTVector geomNormal;
    CRTVector shadingNormal;
    CRTVector baryCoords;
    int materialIDx;
    int objectIDx;
    int triangleIDx;
    float t;


    CRTMesh* intersectedObject;
    CRTTriangle intersectionTriangle;
    Intersection() {};
};

#endif