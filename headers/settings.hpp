#ifndef CRTSETTINGS
#define CRTSETTINGS
#include "crtVector.hpp"
class CRTSettings {
    public:
    CRTSettings(CRTVector backgroundColor, int width, int height) : backgroundColor(backgroundColor),imageWidth(width),imageHeight(height) {}
    CRTVector backgroundColor;
    int imageWidth;
    int imageHeight;

    private:
};
#endif