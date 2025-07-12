#include "../headers/ray.hpp"
#include <iostream>


CRTVector CRTRay::reflect(const CRTVector& rayDirection, CRTVector normal) {
    CRTVector reflectedRay(0.f);

    float dotRN = CRTVector::dot(rayDirection, normal);
    return rayDirection-normal*2.f*dotRN;
}

CRTVector CRTRay::refract(const CRTVector& rayDirection,const CRTVector& normal,const float relativeIOR) {

    //check if Ray is exiting an object


    //calculate entry angle
    float cosAlpha = -CRTVector::dot(normal,rayDirection.normalize());
    float sinBeta = sqrt(1.f-(cosAlpha*cosAlpha))*relativeIOR;
    float cosBeta = cos(asin(sinBeta));
    //std::cout << acos(cosAlpha) << std::endl;
    //std::cout << asin(sinBeta) << std::endl;
    CRTVector R =cosBeta*(normal*(-1.f)) + ((rayDirection + cosAlpha*normal).normalize()*sinBeta);
    return R;
}