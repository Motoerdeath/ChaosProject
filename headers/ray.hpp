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
    //bool intersectTriangle(CRTTriangle triangle, float& t, bool hitBackside);

    const bool intersectBoundingBox(const AABB& aabb) const ;
    const bool intersectTriangle(CRTTriangle triangle, float& t, bool hitBackside) const;
    const bool intersectTriangle(CRTTriangle* triangle, float& t, bool hitBackside) const;
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
    const CRTVector refract(const CRTVector& normal,const float relativeIOR) const;
    const CRTVector reflect(const CRTVector& normal) const;
    const bool isTotallyInternallyReflected(const CRTVector& normal,const float entryIOR, const float exitIOR) const;
    int rayDepth = 0;
    RayType type;
    private:
    
};

#endif
