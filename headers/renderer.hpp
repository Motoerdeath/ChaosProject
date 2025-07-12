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
    CRTVector traceRay(const CRTRay& ray,const float maxT=std::numeric_limits<float>::max());
    bool findIntersection(const CRTRay& ray,Intersection& isect,const float maxT=std::numeric_limits<float>::max());

    CRTVector calculateShading(const CRTRay& ray,Intersection& isect);
    CRTVector constantShading(const CRTRay& ray,Intersection& isect);
    CRTVector flatShading(const CRTRay& ray,Intersection& isect);
    CRTVector smoothShading(const CRTRay& ray,Intersection& isect);
    static float fresnel(const CRTRay& ray,const CRTVector& normal);
    static float fresnel_schlick(const CRTRay& ray,const CRTVector& normal,const float etai,const float eta2);
    void createRenderImage();
    PPMImage image;
    bool intersect(const CRTRay& ray,Intersection& isect);
    int maxDepth = 6;
    CRTVector diffuseShading(const CRTRay& ray,Intersection& isect);
    CRTVector reflectiveShading(const CRTRay& ray,Intersection& isect);
    CRTVector refractiveShading(const CRTRay& ray,Intersection& isect);
};
#endif