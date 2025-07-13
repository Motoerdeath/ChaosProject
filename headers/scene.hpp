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
        void parse();
        void importSettings(rapidjson::Document& doc);
        void importCamera(rapidjson::Document& doc, int width, int height);
        void importObjects(rapidjson::Document& doc);
        void importLights(rapidjson::Document& doc);
        void importMaterials(rapidjson::Document& doc);
        void importTextures(rapidjson::Document& doc);
        const CRTVector getBackgroundColor();
        CRTSettings* getSettings();
        CRTCamera* getCamera();
        Material getMaterial(int materialIDx);

        CRTCamera sceneCamera;
        std::string sceneFilePath;
        std::vector<CRTMesh> sceneObjects;
        std::vector<Light> sceneLights;
        std::vector<Material> sceneMaterials;
        PPMImage sceneImage;
        CRTSettings sceneSettings;
        private:
};
#endif