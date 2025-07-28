#ifndef CRTRENDERER
#define CRTRENDERER

#include "accelerationStructure.hpp"
#include "crtVector.hpp"
#include "globalSettings.hpp"
#include "ppmImage.hpp"
#include "ray.hpp"
#include "scene.hpp"
#include "intersection.hpp"
#include <limits>
#include <memory>
#include "../headers/bucket.hpp"

#include <mutex>
#include <random>
#include <vector>

enum DebugMode {
    None,
    GeometricNormals,
    ShadingNormals,
    BarycentricCoordinates,
    TextureCoordinates,
    HeatMap,
    TriangleView,
};
class CRTRenderer {
    public:
    CRTRenderer() {}
    CRTRenderer(CRTScene* scene);
    void render();
    void renderRegion(const int startX,const int startY,const int regionWidth, const int regionHeight);
    void storeImage(std::string filePathName){ image.storeImageToFile(filePathName);};
    void rebuildAccelerationStructure();

        BucketQueue renderQueue;
    std::mutex bucketMutex;
    std::mutex updateMutex;
    int finishedBuckets =0;
    void setupRNG();

    bool useAccelerationStructure = ACCELERATION;
    bool useGlobalIllumination = GLOBALILLUMINATION;
    bool useMultiThreading = MULTITHREADING;
    DebugMode debug = None;

    private:
    PPMImage image;
    CRTScene* scene;


    CRTVector traceRay(const CRTRay& ray,const float maxT=std::numeric_limits<float>::max());

    CRTVector calculateShading(const CRTRay& ray,Intersection& isect);
    CRTVector constantShading(const CRTRay& ray,Intersection& isect);
    static float fresnel(const CRTRay& ray,const CRTVector& normal);
    static float fresnel_schlick(const CRTRay& ray,const CRTVector& normal,const float etai,const float eta2);
    void createRenderImage();
    
    bool intersect(const CRTRay& ray,Intersection& isect,const float maxT=std::numeric_limits<float>::max());
    CRTVector diffuseShading(const CRTRay& ray,Intersection& isect);
    CRTVector diffuseShadingGI(const CRTRay& ray,Intersection& isect);
    CRTVector reflectiveShading(const CRTRay& ray,Intersection& isect);
    CRTVector refractiveShading(const CRTRay& ray,Intersection& isect);
    CRTRay createReflectionRay(const CRTRay& ray, const CRTVector& position, const CRTVector& normal);
    CRTRay createRefractionRay(const CRTRay& ray, const CRTVector& position, const CRTVector& normal, const float relativeIOR);
    const float shadowbias = 0.01f;
    const float diffuseRayBias = 0.0001f;
    const float reflectionBias = 0.001f;
    const float refractionBias = 0.001f;

    float fade(float low, float high, float value);
    CRTVector temperature(float intensity);
    AccelerationStructure as;
    
    CRTVector getAlbedo(Material mat, Intersection& isect);
    
    void renderSingleThreaded(); 
    void renderMultiThreaded();

    std::mt19937 mt;
    std::uniform_real_distribution<float> dist;
    CRTVector triIDtoColor(int triIdx);
};
#endif