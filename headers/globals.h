#include "glm/glm.hpp"

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
    CRTVector rayOrigin;
    CRTVector rayDirection;
};

struct GridCell {
    glm::vec3 colorValue;
    bool randomize;
};


