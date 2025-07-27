#ifndef CRTRAY
#define CRTRAY
#include "crtVector.hpp"
#include "triangle.hpp"
#include "AABB.hpp"
//#include <bit>


enum RayType{
    Invalid,
    CameraRay,
    ShadowRay,
    ReflectionRay,
    RefractionRay
};
class CRTRay {
    public:
    CRTVector rayOrigin;
    CRTVector rayDirection;
    CRTRay() : rayOrigin(0.f),rayDirection(0.f) {}
    CRTRay(CRTVector origin, CRTVector direction) :  rayOrigin(origin), rayDirection(direction) {}
    bool intersectTriangle(CRTTriangle triangle, float& t, bool hitBackside) {

        constexpr float EPSILON = 0.001f;
        CRTVector normal = triangle.calculateTriangleNormal();
        //check if Ray is not parallel and if ray hits it from the front
        bool keepIsect;
        if(hitBackside) {
            keepIsect = CRTVector::dot(rayDirection, normal) != 0;
        } else {
            keepIsect = CRTVector::dot(rayDirection, normal) < 0;
        }
        if(keepIsect) {
            //return true;
            
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

    static bool intersectBoundingBox(const CRTRay& ray, const AABB& aabb);
    static bool intersectTriangle(const CRTRay& ray,CRTTriangle triangle, float& t, bool hitBackside) {

        constexpr float EPSILON = 0.0001f;
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


            //check if intersection point is in triangle, by checking whether it is "on the left" of each triangle side
            if(CRTVector::dot(normal,CRTVector::cross(e0,v0p)) >=-EPSILON && CRTVector::dot(normal,CRTVector::cross(e1,v1p)) >=-EPSILON && CRTVector::dot(normal,CRTVector::cross(e2,v2p)) >=-EPSILON ) {
                t = t1;
                return true;
            }
            
        }
        return false;
    }

    static CRTVector offsetRay(const CRTVector& pos, const CRTVector& normal) {
        /*
        int x_i = int(256.f*normal.x);
        int y_i = int(256.f*normal.y);
        int z_i = int(256.f*normal.z);
        const float origin =1.f/32.f;
        const float float_scale = 1.f/65536.f;
        CRTVector p_i(std::bit_cast<float>(std::bit_cast<int>(pos.x) + ((pos.x < 0.f) ? -x_i : x_i)),
                        std::bit_cast<float>(std::bit_cast<int>(pos.y) + ((pos.y < 0.f) ? -y_i : y_i)),
                        std::bit_cast<float>(std::bit_cast<int>(pos.z) + ((pos.z < 0.f) ? -z_i : z_i)));
        return CRTVector(std::abs(pos.x)<origin ? pos.x+float_scale*normal.x : p_i.x,
                        std::abs(pos.y)<origin ? pos.y+float_scale*normal.y : p_i.y,
                        std::abs(pos.z)<origin ? pos.z+float_scale*normal.z : p_i.z);
                        */
                        return CRTVector(0.f);
    }
    static CRTVector reflect(const CRTVector& ray, CRTVector normal);
    static CRTVector refract(const CRTVector& ray,const CRTVector& normal,const float relativeIOR);
    int rayDepth = 0;
    RayType type;
    private:
    
};

#endif
