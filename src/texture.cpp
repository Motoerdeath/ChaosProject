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

    int uFactor = static_cast<int>(uvCoordinatesInterpolated.x/squareSize);
    int vFactor = static_cast<int>(uvCoordinatesInterpolated.y/squareSize);
    if(uFactor % 2 == 0 && vFactor % 2 == 0) return colorA;
    if(uFactor % 2 == 1 && vFactor % 2 == 0) return colorB;
    if(uFactor % 2 == 0 && vFactor % 2 == 1) return colorB;
    if(uFactor % 2 == 1 && vFactor % 2 == 1) return colorA;
    return CRTVector{0.f};
}

CRTVector Texture::sampleBitmapTexture(CRTVector uvCoordinatesInterpolated) {
    int u = static_cast<int>(uvCoordinatesInterpolated.x*bitmapWidth);
    int v = static_cast<int>(uvCoordinatesInterpolated.y*bitmapHeight);
    return buffer[v][u];
}

CRTVector EdgeTexture::sample(CRTVector baryCoords) {
    if(baryCoords.x >= edgeWidth && baryCoords.y >= edgeWidth && baryCoords.z >= edgeWidth) {
        return innerColor;
    } else {
        return edgeColor;
    }
}

CRTVector CheckersTexture::sample(CRTVector uvCoords) {
    int uFactor = static_cast<int>(uvCoords.x/squareSize);
    int vFactor = static_cast<int>(uvCoords.y/squareSize);
    if(uFactor % 2 == 0 && vFactor % 2 == 0) return colorA;
    if(uFactor % 2 == 1 && vFactor % 2 == 0) return colorB;
    if(uFactor % 2 == 0 && vFactor % 2 == 1) return colorB;
    if(uFactor % 2 == 1 && vFactor % 2 == 1) return colorA;
    return CRTVector{0.f};
}

CRTVector BitmapTexture::sample(CRTVector uvCoords) {
    int u = static_cast<int>(uvCoords.x*bitmapWidth);
    int v = static_cast<int>(uvCoords.y*bitmapHeight);
    return buffer[v][u];
}