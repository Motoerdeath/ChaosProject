#ifndef GLOBALS
#define GLOBALS

#include "glm/glm.hpp"
#include "../headers/crtVector.hpp"
//TODO: change struct into class with mathematical support
struct CRTVector {
    float x;
    float y;
    float z;
};

struct CRTColor {
    int r;
    int g;
    int b;
};

struct CRTRay {
    
    CRTVector2 rayOrigin;
    CRTVector2 rayDirection;
    CRTRay() : rayOrigin(0.f),rayDirection(0.f) {}
    CRTRay(CRTVector2 origin, CRTVector2 direction) :  rayOrigin(origin), rayDirection(direction) {}
};

struct GridCell {
    glm::vec3 colorValue;
    bool randomize;
};


#endif