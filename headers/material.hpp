#ifndef CRTMATERIAL
#define CRTMATERIAL


#include "crtVector.hpp"
enum MaterialType {
    diffuse,
    reflective
};
enum RenderingStyle {
    constant,
    flat,
    smooth
};
class Material{
    public: 
        Material();
    private:
        MaterialType type;
        CRTVector albedo;
        RenderingStyle style;
};

#endif