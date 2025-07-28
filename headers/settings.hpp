#ifndef CRTSETTINGS
#define CRTSETTINGS
#include "crtVector.hpp"
class CRTSettings {
    public:
    CRTSettings() : backgroundColor(CRTVector(0.f)), imageWidth(1920), imageHeight(1080) {}
    CRTSettings(CRTVector backgroundColor, int width, int height, int bucketSize) : backgroundColor(backgroundColor),imageWidth(width),imageHeight(height),bucketSize(bucketSize) {}
    CRTVector backgroundColor;
    int imageWidth;
    int imageHeight;
    int bucketSize;
    bool multithreading;
    bool globalIllumination;
    bool reflections;
    bool refractions;

    private:
};
#endif