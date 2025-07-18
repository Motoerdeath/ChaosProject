#include "../headers/ray.hpp"
#include <algorithm>
#include <iostream>
#include <limits>


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




bool CRTRay::intersectBoundingBox(const CRTRay &ray, const AABB &aabb) {
    float smallestDistance = std::numeric_limits<float>::max();
    CRTVector pos{0.f};
    CRTVector tMin = (aabb.min-ray.rayOrigin)/ray.rayDirection;
    CRTVector tMax = (aabb.max-ray.rayOrigin)/ray.rayDirection;

    CRTVector t1 = CRTVector(std::min(tMin.x,tMax.x),std::min(tMin.y,tMax.y),std::min(tMin.z,tMax.z));
    CRTVector t2 = CRTVector(std::max(tMin.x,tMax.x),std::max(tMin.y,tMax.y),std::max(tMin.z,tMax.z));

    float tNear = std::max(std::max(t1.x,t1.y),t1.z);
    float tFar = std::min(std::min(t2.x,t2.y),t2.z);
    return tNear <= tFar && tFar >=0.f;

    float tx1 = (aabb.min.x - ray.rayOrigin.x) / ray.rayDirection.x;
    float tx2 = (aabb.max.x - ray.rayOrigin.x) / ray.rayDirection.x;
    float ty1 = (aabb.min.y - ray.rayOrigin.y) / ray.rayDirection.y;
    float ty2 = (aabb.max.y - ray.rayOrigin.y) / ray.rayDirection.y;
    float tz1 = (aabb.min.z - ray.rayOrigin.z) / ray.rayDirection.z;
    float tz2 = (aabb.max.z - ray.rayOrigin.z) / ray.rayDirection.z;
    if(tx1 > 0.f) {
        CRTVector p = ray.rayOrigin + tx1*ray.rayDirection;
        if(p.x <= aabb.max.x && p.x >= aabb.min.x) {
            smallestDistance = std::min(tx1,smallestDistance);
        }
    }
    if(tx2 > 0.f) {
        CRTVector p = ray.rayOrigin + tx2*ray.rayDirection;
        if(p.x <= aabb.max.x && p.x >= aabb.min.x) {
            smallestDistance = std::min(tx2,smallestDistance);
        }
    }
    if(ty1 > 0.f) {
        CRTVector p = ray.rayOrigin + ty1*ray.rayDirection;
        if(p.y <= aabb.max.y && p.y >= aabb.min.y) {
            smallestDistance = std::min(ty1,smallestDistance);
        }
    }
    if(ty2 > 0.f) {
        CRTVector p = ray.rayOrigin + ty2*ray.rayDirection;
        if(p.y <= aabb.max.y && p.y >= aabb.min.y) {
            smallestDistance = std::min(ty2,smallestDistance);
        }
    }
    if(tz1 > 0.f) {
        CRTVector p = ray.rayOrigin + tz1*ray.rayDirection;
        if(p.z <= aabb.max.z && p.z >= aabb.min.z) {
            smallestDistance = std::min(tz1,smallestDistance);
        }
    }
    if(tz2 > 0.f) {
        CRTVector p = ray.rayOrigin + tz2*ray.rayDirection;
        if(p.z <= aabb.max.z && p.z >= aabb.min.z) {
            smallestDistance = std::min(tz2,smallestDistance);
        }
    }
    if(smallestDistance >0.f) {
        return true;
    } else {
        return false;
    }
}