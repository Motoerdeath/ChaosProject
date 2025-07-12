#ifndef CRTRENDERER
#define CRTRENDERER

#include "crtVector.hpp"
#include "ppmImage.hpp"
#include "scene.hpp"
#include "intersection.hpp"
#include <limits>
#include <memory>


enum DebugMode {
    None,
    GeometricNormals,
    ShadingNormals,
    BarycentricCoordinates,
    TextureCoordinates,
    HeatMap,
};
class CRTRenderer {
    public:
    CRTRenderer() {}
    CRTRenderer(CRTScene* scene);
    void render();
    void storeImage(std::string filePathName){ image.storeImageToFile(filePathName);};
    private:
    CRTScene* scene;
    std::unique_ptr<CRTScene> scene2;
    CRTVector traceCameraRay(const CRTRay& ray);
    bool traceShadowRay(const CRTRay& ray,const float maxT=std::numeric_limits<float>::max());
    CRTVector traceReflectionRay(const CRTRay& ray);
    CRTVector traceRefractionRay(const CRTRay& ray);
    CRTVector traceRay(const CRTRay& ray);
    bool findIntersection(const CRTRay& ray,Intersection& isect);

    CRTVector calculateShading(const CRTRay& ray,Intersection& isect);
    CRTVector constantShading(const CRTRay& ray,Intersection& isect);
    CRTVector flatShading(const CRTRay& ray,Intersection& isect);
    CRTVector smoothShading(const CRTRay& ray,Intersection& isect);
    static float fresnel(const CRTRay& ray,Intersection& isect);
    void createRenderImage();
    PPMImage image;
    bool intersect(const CRTRay& ray,Intersection& isect);
    int maxDepth = 5;
    CRTVector diffuseShading(const CRTRay& ray,Intersection& isect);
    CRTVector reflectiveShading(const CRTRay& ray,Intersection& isect);
    CRTVector refractiveShading(const CRTRay& ray,Intersection& isect);
};

class ConstantRenderer : public CRTRenderer {

};

class FlatRenderer : public CRTRenderer {

};


class SmoothRenderer : public CRTRenderer {

};
#endif