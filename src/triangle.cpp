#include "../headers/triangle.hpp"

CRTVector CRTTriangle::calculateBarycentricCoordinates(const CRTTriangle &triangle, const CRTVector &position) {
    CRTVector v0p = position-triangle.v0;
    CRTVector v0v1 = triangle.v1-triangle.v0;
    CRTVector v0v2 = triangle.v2-triangle.v0;
    
    /*
    float areaTri = CRTVector::dot(triangle.normal, CRTVector::cross(v0v1,v0v2));
    float areaU = CRTVector::dot(triangle.normal, CRTVector::cross(v0p,v0v2));
    float areaV = CRTVector::dot(triangle.normal, CRTVector::cross(v0v1,v0p));
    */
    float areaTri = CRTVector::cross(v0v1,v0v2).length()/2.f;
    float areaU = CRTVector::cross(v0p,v0v2).length()/2.f;
    float areaV = CRTVector::cross(v0v1,v0p).length()/2.f;
    
    float u = areaU/areaTri;
    float v = areaV/areaTri;
    float z = 1.f-u-v;
    return CRTVector(u,v,1.f-u-v);
}
