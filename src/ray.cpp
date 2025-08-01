#include "../headers/ray.hpp"
#include <algorithm>
#include <iostream>
#include <limits>

const CRTVector CRTRay::reflect(const CRTVector& normal)  const {
    float dotRN = CRTVector::dot(rayDirection, normal);
    return rayDirection-normal*2.f*dotRN;
}
const CRTVector CRTRay::refract(const CRTVector& normal,const float relativeIOR) const {

    CRTVector I = rayDirection.normalize();
    float cosAlpha = -1.f*CRTVector::dot(I, normal);
    float sinBeta = sqrt(1-cosAlpha*cosAlpha) *relativeIOR;
    float cosBeta = sqrt(1-sinBeta*sinBeta);
    CRTVector C = (I +  cosAlpha*normal).normalize();
    CRTVector B = C*sinBeta;
    CRTVector A = cosBeta * -1.f * normal;
    return A +B;
}

const bool CRTRay::isTotallyInternallyReflected(const CRTVector& normal,const float entryIOR, const float exitIOR) const {
    CRTVector I = rayDirection.normalize();
    float cosAlpha = -1.f*CRTVector::dot(I, normal);
    return sqrt(1-cosAlpha*cosAlpha)> exitIOR/entryIOR;

}


const bool CRTRay::intersectBoundingBox(const AABB& aabb) const {
    float smallestDistance = std::numeric_limits<float>::max();
    CRTVector pos{0.f};
    CRTVector tMin = (aabb.min-rayOrigin)/rayDirection;
    CRTVector tMax = (aabb.max-rayOrigin)/rayDirection;

    CRTVector t1 = CRTVector(std::min(tMin.x,tMax.x),std::min(tMin.y,tMax.y),std::min(tMin.z,tMax.z));
    CRTVector t2 = CRTVector(std::max(tMin.x,tMax.x),std::max(tMin.y,tMax.y),std::max(tMin.z,tMax.z));

    float tNear = std::max(std::max(t1.x,t1.y),t1.z);
    float tFar = std::min(std::min(t2.x,t2.y),t2.z);
    return tNear <= tFar && tFar >=0.f;

    float tx1 = (aabb.min.x - rayOrigin.x) / rayDirection.x;
    float tx2 = (aabb.max.x - rayOrigin.x) / rayDirection.x;
    float ty1 = (aabb.min.y - rayOrigin.y) / rayDirection.y;
    float ty2 = (aabb.max.y - rayOrigin.y) / rayDirection.y;
    float tz1 = (aabb.min.z - rayOrigin.z) / rayDirection.z;
    float tz2 = (aabb.max.z - rayOrigin.z) / rayDirection.z;
    if(tx1 > 0.f) {
        CRTVector p = rayOrigin + tx1*rayDirection;
        if(p.x <= aabb.max.x && p.x >= aabb.min.x) {
            smallestDistance = std::min(tx1,smallestDistance);
        }
    }
    if(tx2 > 0.f) {
        CRTVector p = rayOrigin + tx2*rayDirection;
        if(p.x <= aabb.max.x && p.x >= aabb.min.x) {
            smallestDistance = std::min(tx2,smallestDistance);
        }
    }
    if(ty1 > 0.f) {
        CRTVector p = rayOrigin + ty1*rayDirection;
        if(p.y <= aabb.max.y && p.y >= aabb.min.y) {
            smallestDistance = std::min(ty1,smallestDistance);
        }
    }
    if(ty2 > 0.f) {
        CRTVector p = rayOrigin + ty2*rayDirection;
        if(p.y <= aabb.max.y && p.y >= aabb.min.y) {
            smallestDistance = std::min(ty2,smallestDistance);
        }
    }
    if(tz1 > 0.f) {
        CRTVector p = rayOrigin + tz1*rayDirection;
        if(p.z <= aabb.max.z && p.z >= aabb.min.z) {
            smallestDistance = std::min(tz1,smallestDistance);
        }
    }
    if(tz2 > 0.f) {
        CRTVector p = rayOrigin + tz2*rayDirection;
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



const bool CRTRay::intersectTriangle(CRTTriangle triangle, float& t, bool hitBackside) const {

    constexpr float EPSILON = 0.0001f;
    CRTVector normal = triangle.calculateTriangleNormal();
    //check if Ray is not parallel and if ray hits it from the front
    bool keepIsect;
    if(hitBackside) {
        keepIsect = CRTVector::dot(rayDirection, normal) != 0;
    } else {
        keepIsect = CRTVector::dot(rayDirection, normal) < 0;
    }
    if(keepIsect) {
        
        //find intersection point
        float rpLength = CRTVector::dot(triangle.v0-rayOrigin,normal);
        float t1 = rpLength/CRTVector::dot(rayDirection,normal);
        
        if(t1 < EPSILON) {
            return false;
        }
            
        CRTVector intersection = rayOrigin + rayDirection*t1;

        CRTVector e0 = triangle.v1 - triangle.v0;
        CRTVector e1 = triangle.v2 - triangle.v1;
        CRTVector e2 = triangle.v0 - triangle.v2;
        CRTVector v0p = intersection-triangle.v0;
        CRTVector v1p = intersection-triangle.v1;
        CRTVector v2p = intersection-triangle.v2;
        //check if intersection point is in triangle, by checking whether it is "on the left" of each triangle side
        if(CRTVector::dot(normal,CRTVector::cross(e0,v0p)) >=-EPSILON && CRTVector::dot(normal,CRTVector::cross(e1,v1p)) >=-EPSILON && CRTVector::dot(normal,CRTVector::cross(e2,v2p)) >=-EPSILON ) {
            t = t1;
            return true;
        }
        
    }
    return false;
}

const bool CRTRay::intersectTriangle(CRTTriangle* triangle, float& t, bool hitBackside) const {

    constexpr float EPSILON = 0.0001f;
    CRTVector normal = triangle->normal;
    //check if Ray is not parallel and if ray hits it from the front
    bool keepIsect;
    if(hitBackside) {
        keepIsect = CRTVector::dot(rayDirection, normal) != 0;
    } else {
        keepIsect = CRTVector::dot(rayDirection, normal) < 0;
    }
    if(keepIsect) {
        
        //find intersection point
        float rpLength = CRTVector::dot(triangle->v0-rayOrigin,normal);
        float t1 = rpLength/CRTVector::dot(rayDirection,normal);
        
        if(t1 < EPSILON) {
            return false;
        }
            
        CRTVector intersection = rayOrigin + rayDirection*t1;

        CRTVector e0 = triangle->v1 - triangle->v0;
        CRTVector e1 = triangle->v2 - triangle->v1;
        CRTVector e2 = triangle->v0 - triangle->v2;
        CRTVector v0p = intersection-triangle->v0;
        CRTVector v1p = intersection-triangle->v1;
        CRTVector v2p = intersection-triangle->v2;
        //check if intersection point is in triangle, by checking whether it is "on the left" of each triangle side
        if(CRTVector::dot(normal,CRTVector::cross(e0,v0p)) >=-EPSILON && CRTVector::dot(normal,CRTVector::cross(e1,v1p)) >=-EPSILON && CRTVector::dot(normal,CRTVector::cross(e2,v2p)) >=-EPSILON ) {
            t = t1;
            return true;
        }
        
    }
    return false;
}
