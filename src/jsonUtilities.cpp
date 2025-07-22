#include "../headers/jsonUtilities.hpp"
#include "../headers/crtVector.hpp"
#include "../headers/matrix.hpp"
#include "../include/rapidjson/rapidjson.h"
#include "../include/rapidjson/Document.h"
#include <cassert>
#include <string>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "../include/stbi/stb_image.h"
CRTVector loadVector(const rapidjson::Value::ConstArray& bgColorArr) {
    assert(bgColorArr.Size() == 3);
    return CRTVector(static_cast<float>(bgColorArr[0].GetDouble()),
                    static_cast<float>(bgColorArr[1].GetDouble()),
                    static_cast<float>(bgColorArr[2].GetDouble()));
}


//loads a 3x3 matrix
CRTMatrix loadMatrix(const rapidjson::Value::ConstArray& rotMatrix) {
    assert(rotMatrix.Size() == 9);
    CRTVector v0(static_cast<float>(rotMatrix[0].GetDouble()),
                    static_cast<float>(rotMatrix[1].GetDouble()),
                    static_cast<float>(rotMatrix[2].GetDouble()));
    CRTVector v1(static_cast<float>(rotMatrix[3].GetDouble()),
                    static_cast<float>(rotMatrix[4].GetDouble()),
                    static_cast<float>(rotMatrix[5].GetDouble()));
    CRTVector v2(static_cast<float>(rotMatrix[6].GetDouble()),
                    static_cast<float>(rotMatrix[7].GetDouble()),
                    static_cast<float>(rotMatrix[8].GetDouble()));
    return CRTMatrix(v0,v1,v2);
}

std::vector<CRTVector> loadVertices(const rapidjson::Value::ConstArray& verticesArr) {
    std::vector<CRTVector> vertices;
    assert(verticesArr.Size() % 3 == 0);
    for(int i =0; i < verticesArr.Size();i+=3) {
        CRTVector v(static_cast<float>(verticesArr[i].GetDouble()),
                    static_cast<float>(verticesArr[i+1].GetDouble()),
                    static_cast<float>(verticesArr[i+2].GetDouble()));
        vertices.push_back(v);
    }
    return vertices;
}

std::vector<int> loadIndices(const rapidjson::Value::ConstArray& indicesArr) {
    std::vector<int> indices;
    assert(indicesArr.Size() % 3 == 0);
    for(int i =0; i < indicesArr.Size();i++) {
        int index = static_cast<int>(indicesArr[i].GetInt());
        indices.push_back(index);
    }
    return indices;
}

std::vector<CRTVector> loadTextureCoordinates(const rapidjson::Value::ConstArray& uvCoordsArr) {
    std::vector<CRTVector> textureCoords;
    assert(uvCoordsArr.Size() % 3 == 0);
    for(int i =0; i < uvCoordsArr.Size();i+=3) {
        CRTVector v(static_cast<float>(uvCoordsArr[i].GetDouble()),
                    static_cast<float>(uvCoordsArr[i+1].GetDouble()),
                    static_cast<float>(uvCoordsArr[i+2].GetDouble()));
        textureCoords.push_back(v);
    }
    return textureCoords;
}
std::vector<std::vector<CRTVector>> loadImageFromFile(const std::string filePath, int& imageWidth, int& imageHeight) {
    std::vector<std::vector<CRTVector>> resultImage;
    unsigned char* image;
    int width, height, n;
    int forceChannels = 3;
    //resultImage.reserve(height,std::vector<CRTVector>(width,CRTVector))

    
    image = stbi_load(filePath.c_str(), &width, &height, &n, forceChannels);
    
    if(image) {
        resultImage.resize(height,std::vector<CRTVector>(width,CRTVector(0.f)));
        for(int y = 0; y < height; y++) {
            for(int x = 0; x < width; x++) {
                resultImage[height-1-y][x] = CRTVector(static_cast<float>(image[y*width*n+x*n])/255.f,static_cast<float>(image[y*width*n+x*n+1])/255.f,static_cast<float>(image[y*width*n+x*n+2])/255.f);
            }
        }
    } else {
        assert(false);
    }
    stbi_image_free(image);
    imageHeight = height;
    imageWidth = width;
    return resultImage;
}