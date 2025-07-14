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
    private:
        std::string name="";
        TextureType type=invalidTexture;

        //TExture type specific sampling methods
        CRTVector sampleAlbedoTexture(CRTVector uvCoordinates);
        CRTVector sampleCheckersTexture(CRTVector uvCoordinates);
        CRTVector sampleEdgesTexture(CRTVector uvCoordinates);
        CRTVector sampleBitmapTexture(CRTVector uvCoordinates);

        //texture type specific variables
        CRTVector albedo{0.f}; //required for and by Albedo textures
        float edgeWidth = 0.f; //required for and by edge textures
        float checkerCubeSize = 0.f; // required for and by checkers textures
        std::vector<CRTVector> buffer{}; // required for and by Bitmap textures
};

#endif