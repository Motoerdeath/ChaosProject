#ifndef CRTSETTINGS
#define CRTSETTINGS
#include "crtVector.hpp"
class CRTSettings {
    public:
    CRTSettings() : backgroundColor(CRTVector(0.f)), imageWidth(1920), imageHeight(1080) {}
    CRTSettings(CRTVector backgroundColor, int width, int height) : backgroundColor(backgroundColor),imageWidth(width),imageHeight(height) {}
    CRTVector backgroundColor;
    int imageWidth;
    int imageHeight;

    private:
};
#endif