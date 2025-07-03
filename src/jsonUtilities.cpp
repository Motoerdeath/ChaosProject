#include "../headers/jsonUtilities.hpp"
#include "../headers/crtVector.hpp"
#include "../headers/matrix.hpp"
#include "rapidjson/rapidjson.h"
#include "rapidjson/Document.h"
#include <cassert>
#include <string>
#include <vector>
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

