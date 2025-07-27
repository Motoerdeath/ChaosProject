#ifndef CRTLIGHT
#define CRTLIGHT

#include "crtVector.hpp"
class Light {
    public:
        Light() : lightIntensity(1.f),lightPosition(0.f){}
        Light(CRTVector position) : lightIntensity(1.f),lightPosition(position) {}
        Light(CRTVector position,float intensity) : lightIntensity(intensity),lightPosition(position) {}
        Light(CRTVector position,float intensity, CRTVector lightColor) : lightIntensity(intensity),lightPosition(position),lightColor(lightColor) {}

        float lightIntensity;
        CRTVector lightPosition;
        CRTVector lightColor{1.f};
    private:

};
#endif