#include "../headers/ray.hpp"


CRTVector CRTRay::reflect(const CRTVector& ray, CRTVector normal) {
    CRTVector reflectedRay(0.f);

    float dotRN = CRTVector::dot(ray, normal);
    return ray-normal*2.f*dotRN;
}
