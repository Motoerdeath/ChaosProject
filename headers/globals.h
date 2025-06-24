#ifndef GLOBALS
#define GLOBALS

#include "glm/glm.hpp"
#include "../headers/crtVector.hpp"
//TODO: change struct into class with mathematical support
struct CRTColor {
    int r;
    int g;
    int b;
};

struct CRTRay {
    
    CRTVector rayOrigin;
    CRTVector rayDirection;
    CRTRay() : rayOrigin(0.f),rayDirection(0.f) {}
    CRTRay(CRTVector origin, CRTVector direction) :  rayOrigin(origin), rayDirection(direction) {}
};

struct GridCell {
    glm::vec3 colorValue;
    bool randomize;
};


#endif