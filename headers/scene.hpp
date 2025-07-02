#ifndef CRTSCENE
#define CRTSCENE

#include "camera.hpp"
#include "mesh.hpp"
#include "ppmImage.hpp"
#include "settings.hpp"
#include <string>

class CRTScene {
    public:
        CRTScene(std::string sceneFileName);

        void parseSceneFile(const std::string& sceneFileName);
        void render();
    private:
    std::string sceneFilePath;
    CRTCamera camera;
    std::vector<CRTMesh> objects;
    std::vector<int> lights;
    PPMImage image;
    CRTSettings settings;
};
#endif