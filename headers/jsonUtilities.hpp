#ifndef JSONUTILITIES
#define JSONUTILITIES
#include "../headers/crtVector.hpp"
#include "../headers/matrix.hpp"
#include "rapidjson/Document.h"
#include <cassert>
#include <vector>

//Values
CRTVector loadVector(const rapidjson::Value::ConstArray& bgColorArr);
//loads a 3x3 matrix
CRTMatrix loadMatrix(const rapidjson::Value::ConstArray& rotMatrix);
std::vector<CRTVector> loadVertices(const rapidjson::Value::ConstArray& verticesArr);
std::vector<int> loadIndices(const rapidjson::Value::ConstArray& indicesArr);
#endif