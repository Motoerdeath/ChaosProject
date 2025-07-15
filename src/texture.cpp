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

CRTVector Texture::sampleAlbedoTexture(CRTVector baryCoords) {
    return albedo;
}

CRTVector Texture::sampleEdgesTexture(CRTVector baryCoords) {
    if(baryCoords.x >= edgeWidth && baryCoords.y >= edgeWidth && baryCoords.z >= edgeWidth) {
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

CRTVector Texture::sampleBitmapTexture(CRTVector uvCoordinatesInterpolated) {
    int u = static_cast<int>(uvCoordinatesInterpolated.x*bitmapWidth);
    int v = static_cast<int>(uvCoordinatesInterpolated.y*bitmapHeight);
    return buffer[u][v];
}