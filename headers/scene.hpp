#ifndef CRTSCENE
#define CRTSCENE

#include "camera.hpp"
#include "crtVector.hpp"
#include "mesh.hpp"
#include "ppmImage.hpp"
#include "settings.hpp"
#include <string>
#include "light.hpp"
#include "../include/rapidjson/document.h"
#include "material.hpp"
#include "texture.hpp"



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
        Texture* getTexture(int textureIDx);

        CRTCamera sceneCamera;
        std::string sceneFilePath;
        std::vector<CRTMesh> sceneObjects;
        std::vector<Light> sceneLights;
        std::vector<Material> sceneMaterials;
        std::vector<Texture> sceneTextures;
        PPMImage sceneImage;
        CRTSettings sceneSettings;
        std::vector<std::vector<CRTVector>> textureImage;
        private:
};
#endif