#ifndef UTILITIES
#define UTILITIES
#include "../headers/crtVector.hpp"
#include "rapidjson/rapidjson.h"
#include "rapidjson/Document.h"
#include <cassert>
CRTVector loadVector(const rapidjson::Value::ConstArray& bgColorArr) {
    assert(bgColorArr.Size() == 3);
    return CRTVector(static_cast<float>(bgColorArr[0].GetDouble()),
                    static_cast<float>(bgColorArr[1].GetDouble()),
                    static_cast<float>(bgColorArr[2].GetDouble()));
}

#endif