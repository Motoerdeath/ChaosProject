#ifndef CRTTEXTURE
#define CRTTEXTURE

#include "crtVector.hpp"
#include "intersection.hpp"
#include <string>
#include <vector>

class Texture
{
    public:
        Texture(){}
        Texture(std::string name) : name(name) {}
        virtual CRTVector sample(CRTVector uvCoordinates) = 0;
        virtual CRTVector sample(Intersection& isect) = 0;
        std::string getName() { return name;}
    private:
        std::string name;
};

class AlbedoTexture : public Texture
{
    public:
        AlbedoTexture(std::string name, const CRTVector& albedo) : Texture(name), albedo2(albedo) {}
        CRTVector sample(CRTVector uvCoordinates) override {return albedo2;}
        CRTVector sample(Intersection& isect) override {return sample(isect.baryCoords);}
        
    private:
        CRTVector albedo2;
};

class EdgeTexture : public Texture
{
    public:
        EdgeTexture(std::string name, float edgeWidth, const CRTVector& innerColor, const CRTVector& edgeColor) :
            Texture(name), edgeWidth(edgeWidth), innerColor(innerColor), edgeColor(edgeColor) {}
        CRTVector sample(CRTVector uvCoordinates) override;
        CRTVector sample(Intersection& isect) override {return sample(isect.baryCoords);}
    private:
        float edgeWidth;
        CRTVector edgeColor;
        CRTVector innerColor;
};

class CheckersTexture : public Texture
{
    public:
        CheckersTexture(std::string name, float squareSize, const CRTVector& colorA, const CRTVector& colorB) :
            Texture(name), squareSize(squareSize), colorA(colorA), colorB(colorB) {}
        CRTVector sample(CRTVector uvCoordinates) override;
        CRTVector sample(Intersection& isect) override {return sample(isect.textureCoords);}
    private:
        float squareSize;
        CRTVector colorA;
        CRTVector colorB;
};

class BitmapTexture : public Texture
{
    public:
        BitmapTexture(std::string name,std::vector<std::vector<CRTVector>> buffer, int bitmapHeight, int bitmapWidth) :
            Texture(name),bitmapHeight(bitmapHeight),bitmapWidth(bitmapWidth),buffer(buffer) {}
        CRTVector sample(CRTVector uvCoordinates) override;
        CRTVector sample(Intersection& isect) override {return sample(isect.textureCoords);}
    private:
        int bitmapHeight;
        int bitmapWidth;
        std::vector<std::vector<CRTVector>> buffer;
};
#endif