#ifndef CRTLIGHT
#define CRTLIGHT

#include "crtVector.hpp"
class Light {
    public:
        Light() : lightIntensity(1.f),lightPosition(0.f){}
        Light(CRTVector position) : lightIntensity(1.f),lightPosition(position) {}
        Light(CRTVector position,float intensity) : lightIntensity(intensity),lightPosition(position) {}

        float lightIntensity;
        CRTVector lightPosition;
    private:

};
#endif