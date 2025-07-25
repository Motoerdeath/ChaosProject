#include "../headers/environmentMap.hpp"
#include "../headers/jsonUtilities.hpp"
#include <cassert>


void EnvironmentMap::buildEnvironmentMap(std::vector<std::string> filePaths) {
    assert(filePaths.size==6);


    for(int i = 0; i < filePaths.size(); i++) {
        Texture newTexture;
        newTexture.type = bitmapTexture;
        std::string textureFilePath = filePaths[i];
        int width, height;
        newTexture.buffer = loadImageFromFile("../textures"+ textureFilePath,width,height);
        newTexture.bitmapHeight = height;
        newTexture.bitmapWidth = width;
        cubeMap[i] = newTexture;
    }

}

CRTVector EnvironmentMap::sampleEnvironmentMap(const CRTRay& ray) {
    CRTVector result{0.f};
    CRTVector projectedDir = ray.rayDirection / std::max(std::abs(ray.rayDirection.x),std::max(std::abs(ray.rayDirection.y),std::abs(ray.rayDirection.z)));

    if(projectedDir.x == 1.f) {
        float u = (projectedDir.z +1.f)/2.f;
        float v = (projectedDir.y +1.f)/2.f;
        CRTVector uvCoords{u*cubeMap[0].bitmapWidth,v*cubeMap[0].bitmapHeight,0.f};
        return cubeMap[0].sample(uvCoords);
    } else if (projectedDir.x == -1.f) {
        float u = (projectedDir.z +1.f)/2.f;
        float v = (projectedDir.y +1.f)/2.f;
        CRTVector uvCoords{u*cubeMap[1].bitmapWidth,v*cubeMap[1].bitmapHeight,0.f};
        return cubeMap[0].sample(uvCoords);
    } else if(projectedDir.y == 1.f) {
        float u = (projectedDir.x +1.f)/2.f;
        float v = (projectedDir.z +1.f)/2.f;
        CRTVector uvCoords{u*cubeMap[2].bitmapWidth,v*cubeMap[2].bitmapHeight,0.f};
        return cubeMap[0].sample(uvCoords);
    } else if( projectedDir.y == -1.f) {
        float u = (projectedDir.x +1.f)/2.f;
        float v = (projectedDir.z +1.f)/2.f;
        CRTVector uvCoords{u*cubeMap[3].bitmapWidth,v*cubeMap[3].bitmapHeight,0.f};
        return cubeMap[0].sample(uvCoords);
    } else if(projectedDir.z == 1.f) {
        float u = (projectedDir.x +1.f)/2.f;
        float v = (projectedDir.y +1.f)/2.f;
        CRTVector uvCoords{u*cubeMap[4].bitmapWidth,v*cubeMap[4].bitmapHeight,0.f};
        return cubeMap[0].sample(uvCoords);
    } else if(projectedDir.z == -1.f) {
        float u = (projectedDir.x +1.f)/2.f;
        float v = (projectedDir.y +1.f)/2.f;
        CRTVector uvCoords{u*cubeMap[5].bitmapWidth,v*cubeMap[5].bitmapHeight,0.f};
        return cubeMap[0].sample(uvCoords);
    } else {
        std::printf("catastrophic error in environment mapping");
        assert(false);
    }

    return result;
}