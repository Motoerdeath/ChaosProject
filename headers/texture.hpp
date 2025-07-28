#ifndef CRTTEXTURE
#define CRTTEXTURE

#include "crtVector.hpp"
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
        CRTVector sample(CRTVector uvCoordinates);
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

#endif