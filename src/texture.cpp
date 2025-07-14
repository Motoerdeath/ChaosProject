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
            return CRTVector{0.f};
    }
}

CRTVector Texture::sampleAlbedoTexture(CRTVector uvCoordinates) {
    return albedo;
}

CRTVector Texture::sampleEdgesTexture(CRTVector uvCoordinates) {
    if(uvCoordinates.x >= edgeWidth && uvCoordinates.y >= edgeWidth && uvCoordinates.z >= edgeWidth) {
        return innerColor;
    } else {
        return edgeColor;
    }
}

CRTVector Texture::sampleCheckersTexture(CRTVector uvCoordinatesInterpolated) {

    float sizeFactor = 2.f/squareSize;
    if(uvCoordinatesInterpolated.x < sizeFactor)
    return CRTVector{0.f};
}

CRTVector Texture::sampleBitmapTexture(CRTVector uvCoordinates) {
    return CRTVector{0.f};
}