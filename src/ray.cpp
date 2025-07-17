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

    static bool intersectTriangle(const CRTRay& ray,CRTTriangle triangle, float& t, bool hitBackside) {

        constexpr float EPSILON = 0.00001f;
        CRTVector normal = triangle.calculateTriangleNormal();
        //check if Ray is not parallel and if ray hits it from the front
        bool keepIsect;
        if(hitBackside) {
            keepIsect = CRTVector::dot(ray.rayDirection, normal) != 0;
        } else {
            keepIsect = CRTVector::dot(ray.rayDirection, normal) < 0;
        }
        if(keepIsect) {
            //return true;
            
            //find intersection point
            float rpLength = CRTVector::dot(triangle.v0-ray.rayOrigin,normal);
            float t1 = rpLength/CRTVector::dot(ray.rayDirection,normal);
            
            if(t1 < EPSILON) {
                return false;
            }
                
            CRTVector intersection = ray.rayOrigin + ray.rayDirection*t1;

            CRTVector e0 = triangle.v1 - triangle.v0;
            CRTVector e1 = triangle.v2 - triangle.v1;
            CRTVector e2 = triangle.v0 - triangle.v2;
            CRTVector v0p = intersection-triangle.v0;
            CRTVector v1p = intersection-triangle.v1;
            CRTVector v2p = intersection-triangle.v2;

            //use barycentric coordinates to determine whether point lies within triangle
            /*
            
            std::vector<float> uvCoords = CRTTriangle::calculateBarycentricCoordinates(triangle, intersection);
            if(uvCoords[0] >=0.f && uvCoords[1]>=0.f && uvCoords[0] <= 1.f && uvCoords[1] <= 1.f && uvCoords[0]+uvCoords[1]<= 1.f) {
                t = t1;
                return true;
            }
            */

            //check if intersection point is in triangle, by checking whether it is "on the left" of each triangle side
            if(CRTVector::dot(normal,CRTVector::cross(e0,v0p)) >=-EPSILON && CRTVector::dot(normal,CRTVector::cross(e1,v1p)) >=-EPSILON && CRTVector::dot(normal,CRTVector::cross(e2,v2p)) >=-EPSILON ) {
                t = t1;
                return true;
            }
            
        }
        return false;
    }