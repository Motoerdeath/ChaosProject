#ifndef CRTMATERIAL
#define CRTMATERIAL

#include <string>
#include "crtVector.hpp"
enum MaterialType {
    diffuse,
    reflective
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
    constant,
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
    private:
        MaterialType type;
        CRTVector albedo;
        RenderingStyle style;
};

#endif