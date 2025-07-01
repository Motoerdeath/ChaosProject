#ifndef CRTRAY
#define CRTRAY
#include "../headers/crtVector.hpp"
#include "../headers/triangle.hpp"

class CRTRay {
    public:
    CRTVector rayOrigin;
    CRTVector rayDirection;
    CRTRay() : rayOrigin(0.f),rayDirection(0.f) {}
    CRTRay(CRTVector origin, CRTVector direction) :  rayOrigin(origin), rayDirection(direction) {}
    bool intersectTriangle(CRTTriangle triangle, float& t) {
        CRTVector normal = triangle.calculateTriangleNormal();
        //check if Ray is not parallel and if ray hits it from the front
        if(CRTVector::dot(rayDirection, normal) < 0) {
            //return true;
            
            //find intersection point
            float rpLength = CRTVector::dot(triangle.v0-rayOrigin,normal);
            float t1 = rpLength/CRTVector::dot(rayDirection,normal);
            
            if(t1 < 0) {
                return false;
            }
                
            CRTVector intersection = rayOrigin + rayDirection*t1;

            CRTVector e0 = triangle.v1 - triangle.v0;
            CRTVector e1 = triangle.v2 - triangle.v1;
            CRTVector e2 = triangle.v0 - triangle.v2;
            CRTVector v0p = intersection-triangle.v0;
            CRTVector v1p = intersection-triangle.v1;
            CRTVector v2p = intersection-triangle.v2;
            //check if intersection point is in triangle
            if(CRTVector::dot(normal,CRTVector::cross(e0,v0p)) >=0 && CRTVector::dot(normal,CRTVector::cross(e1,v1p)) >=0 && CRTVector::dot(normal,CRTVector::cross(e2,v2p)) >=0 ) {
                t = t1;
                return true;
            }
            
        }
        return false;
    }
    private:
};

#endif

/*

#ifndef CRTRAY
#define CRTRAY
#include "../headers/crtVector.hpp"
#include "../headers/triangle.hpp"

class CRTRay {
    public:
    CRTVector rayOrigin;
    CRTVector rayDirection;
    CRTRay() : rayOrigin(0.f),rayDirection(0.f) {}
    CRTRay(CRTVector origin, CRTVector direction) :  rayOrigin(origin), rayDirection(direction) {}
    bool intersectTriangle(CRTTriangle triangle, float& t) {
        CRTVector triangleNormal = triangle.calculateTriangleNormal();

        //check if Ray is not parallel and if ray hits it from the front
        if(CRTVector::dot(rayDirection, triangleNormal) < 0) {
            //return true;
            
            //find intersection point
            float rpLength = CRTVector::dot(triangle.v0,triangleNormal);
            float t1 = rpLength/CRTVector::dot(rayDirection,triangleNormal);
            if(t1 < 0) {
                return false;
            }
            CRTVector intersection = rayOrigin + rayDirection*t1;

            CRTVector e0 = triangle.v1 - triangle.v0;
            CRTVector e1 = triangle.v2 - triangle.v1;
            CRTVector e2 = triangle.v0 - triangle.v2;
            CRTVector v0p = intersection-triangle.v0;
            CRTVector v1p = intersection-triangle.v1;
            CRTVector v2p = intersection-triangle.v2;
            //check if intersection point is in triangle
            if(CRTVector::dot(triangleNormal,CRTVector::cross(e0,v0p)) >0 && CRTVector::dot(triangleNormal,CRTVector::cross(e1,v1p)) >0 && CRTVector::dot(triangleNormal,CRTVector::cross(e2,v2p)) >0 ) {
                t = t1;
                return true;
            }
            
        }
        return false;
    }
    private:
};

#endif
*/