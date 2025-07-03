#ifndef CRTSCENE
#define CRTSCENE

#include "camera.hpp"
#include "mesh.hpp"
#include "ppmImage.hpp"
#include "settings.hpp"
#include <string>

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
    std::vector<int> lights;
    PPMImage sceneImage;
    CRTSettings sceneSettings;
};
#endif