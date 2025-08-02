#ifndef CRTTEXTURE
#define CRTTEXTURE

#include "crtVector.hpp"
#include "intersection.hpp"
#include <string>
#include <vector>

enum TextureType {
    invalidTexture,
    albedoTexture,
    edgeTexture,
    checkersTexture,
    bitmapTexture
};
class Texture {
    public:
        Texture(){}
        Texture(std::string name) : name(name) {}
        Texture(std::string name, TextureType type) : name(name), type(type) {}
        CRTVector sample(CRTVector uvCoordinates);
        CRTVector sample2(CRTVector uvCoordinates) {}
        std::string name="";
        TextureType type=invalidTexture;

        //TExture type specific sampling methods
        CRTVector sampleAlbedoTexture(CRTVector uvCoordinates);
        CRTVector sampleCheckersTexture(CRTVector uvCoordinates);
        CRTVector sampleEdgesTexture(CRTVector uvCoordinates);
        CRTVector sampleBitmapTexture(CRTVector uvCoordinates);

        //texture type specific variables:

        //required for and by Albedo textures
        CRTVector albedo{0.f}; 

        //required for and by edge textures
        float edgeWidth = 0.f; 
        CRTVector edgeColor{0.f};
        CRTVector innerColor{1.f};
        // required for and by checkers textures
        float squareSize = 0.f;
        CRTVector colorA{0.f};
        CRTVector colorB{0.f};
        
        // required for and by Bitmap textures
        std::vector<std::vector<CRTVector>> buffer; 
        int bitmapWidth;
        int bitmapHeight;
    private:

};


class Texture2
{
    public:
        Texture2(){}
        Texture2(std::string name) : name(name) {}
        Texture2(std::string name, TextureType type) : name(name) {}
        virtual CRTVector sample(CRTVector uvCoordinates) = 0;
        virtual CRTVector sample(Intersection& isect) = 0;
        std::string getName() { return name;}
    private:
        std::string name;
};

class AlbedoTexture : public Texture2
{
    public:
        AlbedoTexture(std::string name, const CRTVector& albedo) : Texture2(name), albedo2(albedo) {}
        CRTVector sample(CRTVector uvCoordinates) override {return albedo2;}
        CRTVector sample(Intersection& isect) override {return sample(isect.baryCoords);}
        
    private:
        CRTVector albedo2;
};

class EdgeTexture : public Texture2
{
    public:
        EdgeTexture(std::string name, float edgeWidth, const CRTVector& innerColor, const CRTVector& edgeColor) :
            Texture2(name), edgeWidth(edgeWidth), innerColor(innerColor), edgeColor(edgeColor) {}
        CRTVector sample(CRTVector uvCoordinates) override;
        CRTVector sample(Intersection& isect) override {return sample(isect.baryCoords);}
    private:
        float edgeWidth;
        CRTVector edgeColor;
        CRTVector innerColor;
};

class CheckersTexture : public Texture2
{
    public:
        CheckersTexture(std::string name, float squareSize, const CRTVector& colorA, const CRTVector& colorB) :
            Texture2(name), squareSize(squareSize), colorA(colorA), colorB(colorB) {}
        CRTVector sample(CRTVector uvCoordinates) override;
        CRTVector sample(Intersection& isect) override {return sample(isect.textureCoords);}
    private:
        float squareSize;
        CRTVector colorA;
        CRTVector colorB;
};

class BitmapTexture : public Texture2
{
    public:
        BitmapTexture(std::string name,std::vector<std::vector<CRTVector>> buffer, int bitmapHeight, int bitmapWidth) :
            Texture2(name),bitmapHeight(bitmapHeight),bitmapWidth(bitmapWidth),buffer(buffer) {}
        CRTVector sample(CRTVector uvCoordinates) override;
        CRTVector sample(Intersection& isect) override {return sample(isect.textureCoords);}
    private:
        int bitmapHeight;
        int bitmapWidth;
        std::vector<std::vector<CRTVector>> buffer;
};
#endif