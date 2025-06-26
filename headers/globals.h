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



struct GridCell {
    glm::vec3 colorValue;
    bool randomize;
};


#endif