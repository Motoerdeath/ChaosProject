#ifndef CRTSCENE
#define CRTSCENE

#include "camera.hpp"
#include "mesh.hpp"
#include <string>
class CRTScene {
    public:
        CRTScene(std::string sceneFileName);

        void parseSceneFile(const std::string& sceneFileName);
    private:
    std::string sceneFilePath;
    CRTCamera camera;
    std::vector<CRTMesh> objects;
    std::vector<int> lights;
};
#endif