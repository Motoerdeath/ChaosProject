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
            //parse();
        }

        void parseSceneFile(const std::string& sceneFileName);
        void render();
        void parse();
    private:
    std::string sceneFilePath;
    CRTCamera sceneCamera;
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