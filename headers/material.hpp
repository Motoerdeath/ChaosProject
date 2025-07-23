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
        Material(MaterialType mtype,CRTVector albedo, RenderingStyle rStyle, bool backFaceCulling) : type(mtype),albedo(albedo),style(rStyle),backFaceCulling(backFaceCulling) {};
        Material(MaterialType mtype,CRTVector albedo, RenderingStyle rStyle, float ior, bool backFaceCulling) : type(mtype),albedo(albedo),style(rStyle), ior(ior),backFaceCulling(backFaceCulling) {};
        Material(MaterialType mtype,std::string albedoTex, RenderingStyle rStyle, float ior, bool backFaceCulling) : type(mtype),albedoTex(albedoTex),style(rStyle), ior(ior) ,backFaceCulling(backFaceCulling){};
        MaterialType type;
        CRTVector albedo;
        RenderingStyle style;
        float ior = 1.0f;
        std::string albedoTex = "invalid"; //string that references the name of the texture
        bool backFaceCulling = false;
    private:
};

#endif