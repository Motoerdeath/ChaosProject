#include "../headers/AABB.hpp"
#include <iostream>


void AABB::include(CRTTriangle triangle) {

    min = CRTVector(std::min(min.x,triangle.v0.x),std::min(min.y,triangle.v0.y),std::min(min.z,triangle.v0.z));
    min = CRTVector(std::min(min.x,triangle.v1.x),std::min(min.y,triangle.v1.y),std::min(min.z,triangle.v1.z));
    min = CRTVector(std::min(min.x,triangle.v2.x),std::min(min.y,triangle.v2.y),std::min(min.z,triangle.v2.z));

    max = CRTVector(std::max(max.x,triangle.v0.x),std::max(max.y,triangle.v0.y),std::max(max.z,triangle.v0.z));
    max = CRTVector(std::max(max.x,triangle.v1.x),std::max(max.y,triangle.v1.y),std::max(max.z,triangle.v1.z));
    max = CRTVector(std::max(max.x,triangle.v2.x),std::max(max.y,triangle.v2.y),std::max(max.z,triangle.v2.z));
}

void AABB::include(AABB boundingBox) {
    min = CRTVector(std::min(min.x,boundingBox.min.x),std::min(min.y,boundingBox.min.y),std::min(min.z,boundingBox.min.z));
    max = CRTVector(std::max(max.x,boundingBox.max.x),std::max(max.y,boundingBox.max.y),std::min(max.z,boundingBox.max.z));
}