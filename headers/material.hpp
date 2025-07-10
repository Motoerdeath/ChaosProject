#ifndef CRTMATERIAL
#define CRTMATERIAL

#include <string>
#include "crtVector.hpp"
enum MaterialType {
    constant,
    diffuse,
    reflective,
    refractive
};
/*
MaterialType strToType(std::string input){
    if(input == "reflective") {
        return reflective;
    }

    //default diffuse
    return diffuse;
}
    */
enum RenderingStyle {
    flat,
    smooth
};
/*
RenderingStyle strToStyle(std::string input){
    if(input == "constant") {
        return constant;
    }
    if(input == "flat") {
        return flat;
    }
    //default diffuse
    return smooth;
}
    */
class Material{
    public: 
        Material() {};
        Material(MaterialType mtype,CRTVector albedo, RenderingStyle rStyle) : type(mtype),albedo(albedo),style(rStyle) {};
        Material(MaterialType mtype,CRTVector albedo, RenderingStyle rStyle, float ior) : type(mtype),albedo(albedo),style(rStyle), ior(ior) {};
        MaterialType type;
        CRTVector albedo;
        RenderingStyle style;
        float ior = 1.0f;
    private:
};

#endif