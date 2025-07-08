#ifndef CRTSCENE
#define CRTSCENE

#include "camera.hpp"
#include "crtVector.hpp"
#include "mesh.hpp"
#include "ppmImage.hpp"
#include "settings.hpp"
#include <string>
#include "light.hpp"
#include "rapidjson/document.h"
#include "material.hpp"
#include "intersection.hpp"



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
        bool traceRay(CRTRay& ray, Intersection& isect);
        void traceShadowRay(CRTRay& ray);
        void outputResult();
        void outputResult(std::string outputFilePath) { sceneImage.storeImageToFile(outputFilePath);};

        void importSettings(rapidjson::Document& doc);
        void importCamera(rapidjson::Document& doc, int width, int height);
        void importObjects(rapidjson::Document& doc);
        void importLights(rapidjson::Document& doc);
        void importMaterials(rapidjson::Document& doc);

        CRTVector shade(CRTVector pos,CRTVector triangleNormal, Intersection& isect);
        CRTVector flatShade(CRTVector pos,CRTVector triangleNormal);
        CRTCamera sceneCamera;
    private:
    std::string sceneFilePath;
    std::vector<CRTMesh> sceneObjects;
    std::vector<Light> sceneLights;
    std::vector<Material> sceneMaterials;
    PPMImage sceneImage;
    CRTSettings sceneSettings;

    std::vector<CRTVector> colors = {CRTVector(1.f,0.f,0.f),CRTVector(0.f,1.f,0.f),
                                CRTVector(0.f,0.f,1.f),CRTVector(1.f,1.f,1.f),
                                CRTVector(1.f,1.f,0.f),CRTVector(0.f,1.f,1.f),
                                CRTVector(1.f,0.f,1.f),CRTVector(0.f,0.f,0.f)};
};
#endif