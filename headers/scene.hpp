#ifndef CRTSCENE
#define CRTSCENE

#include "camera.hpp"
#include "crtVector.hpp"
#include "mesh.hpp"
#include "object.hpp"
#include "ppmImage.hpp"
#include "settings.hpp"
#include <memory>
#include <string>
#include "light.hpp"
#include "../include/rapidjson/document.h"
#include "material.hpp"
#include "texture.hpp"
#include "triangle.hpp"



class CRTScene {
    public:
        CRTScene(std::string sceneFileName){
            sceneFilePath = sceneFileName;
            sceneCamera = CRTCamera();
        }

        void parseSceneFile(const std::string& sceneFileName);
        void parseSceneFile2(const std::string& sceneFileName);
        void parse();
        void importSettings(rapidjson::Document& doc);
        void importCamera(rapidjson::Document& doc, int width, int height);
        void importObjects(rapidjson::Document& doc);
        void importLights(rapidjson::Document& doc);
        void importMaterials(rapidjson::Document& doc);
        Material importBasicMaterial(rapidjson::Document& doc, int mIdx);
        Material importTextureMaterial(rapidjson::Document& doc, int mIdx);
        void importMaterialsWithTextures(rapidjson::Document& doc);
        void importTextures(rapidjson::Document& doc);
        const CRTVector getBackgroundColor();
        CRTSettings* getSettings();
        CRTCamera* getCamera();
        Material getMaterial(int materialIDx);

        CRTCamera sceneCamera;
        std::string sceneFilePath;
        std::vector<CRTObject> objects;
        std::vector<Light> sceneLights;
        std::vector<Material> sceneMaterials; 
        std::vector<std::unique_ptr<Texture>> sceneTex;
        PPMImage sceneImage;
        CRTSettings sceneSettings;
        std::vector<std::vector<CRTVector>> textureImage;

        std::vector<CRTTriangle> triangleSoup;
        private:
        void createTriangleSoup();
        
};
#endif