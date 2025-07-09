#ifndef CRTRENDERER
#define CRTRENDERER

#include "crtVector.hpp"
#include "scene.hpp"
#include "intersection.hpp"
#include <memory>
class CRTRenderer {
    public:
    CRTRenderer() {}
    CRTRenderer(CRTScene* scene) : scene(scene){scene2 = std::unique_ptr<CRTScene>(scene);};
    void render();
    private:
    CRTScene* scene;
    std::unique_ptr<CRTScene> scene2;
    CRTVector traceCameraRay(const CRTRay& ray);
    bool traceShadowRay(const CRTRay& ray);
    CRTVector traceReflectionRay(const CRTRay& ray);
    CRTVector traceRefractionRay(const CRTRay& ray);
    bool findIntersection(const CRTRay& ray,Intersection& isect);

    CRTVector calculateShading(const CRTRay& ray,Intersection& isect);
    CRTVector constantShading(const CRTRay& ray,Intersection& isect);
    CRTVector flatShading(const CRTRay& ray,Intersection& isect);
    CRTVector smoothShading(const CRTRay& ray,Intersection& isect);
    int maxDepth = 5;
};

class ConstantRenderer : public CRTRenderer {

};

class FlatRenderer : public CRTRenderer {

};


class SmoothRenderer : public CRTRenderer {

};
#endif