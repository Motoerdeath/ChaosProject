#ifndef CRTSCENE
#define CRTSCENE

#include "camera.hpp"
#include "crtVector.hpp"
#include "mesh.hpp"
#include "ppmImage.hpp"
#include "settings.hpp"
#include <string>
#include "light.hpp"



class CRTScene {
    public:
        CRTScene(std::string sceneFileName){
            sceneFilePath = sceneFileName;
            sceneCamera = CRTCamera();
        }

        void parseSceneFile(const std::string& sceneFileName);
        void render();
        void constantRender();
        void flatRender();
        void parse();
        bool isShadowed(CRTVector pos,CRTVector lightDir);
        void traceRay();
        void traceShadowRay();
        CRTVector shade(CRTVector pos,CRTVector triangleNormal);
        CRTCamera sceneCamera;
    private:
    std::string sceneFilePath;
    
    std::vector<CRTMesh> sceneObjects;
    std::vector<Light> sceneLights;
    PPMImage sceneImage;
    CRTSettings sceneSettings;

    std::vector<CRTVector> colors = {CRTVector(1.f,0.f,0.f),CRTVector(0.f,1.f,0.f),
                                CRTVector(0.f,0.f,1.f),CRTVector(1.f,1.f,1.f),
                                CRTVector(1.f,1.f,0.f),CRTVector(0.f,1.f,1.f),
                                CRTVector(1.f,0.f,1.f),CRTVector(0.f,0.f,0.f)};
};
#endif