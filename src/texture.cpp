#include "../headers/texture.hpp"
#include <cassert>


CRTVector Texture::sample(CRTVector uvCoordinates) {
    switch (type) {
        case albedoTexture:
            return sampleAlbedoTexture(uvCoordinates);
            break;
        case bitmapTexture:
            return sampleBitmapTexture(uvCoordinates);
            break;
        case edgeTexture:
            return sampleEdgesTexture(uvCoordinates);
            break;
        case checkersTexture:
            return sampleCheckersTexture(uvCoordinates);
            break;
        default:
            assert(false);
    }
}

CRTVector Texture::sampleAlbedoTexture(CRTVector uvCoordinates) {
    return albedo;
}