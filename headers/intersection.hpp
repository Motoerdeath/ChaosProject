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
    CRTVector textureCoords;
    int materialIDx;
    int objectIDx;
    int triangleIDx;
    int textureIDx{0};

    float t;


    CRTMesh* intersectedObject;
    CRTTriangle intersectionTriangle;
    Intersection() {};
};

#endif