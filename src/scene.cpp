#include "../headers/scene.hpp"
#include "../headers/triangle.hpp"
#include"rapidjson/rapidjson.h"
#include <fstream>
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "../headers/jsonUtilities.hpp"
#include <iostream>

void CRTScene::parse() {
    for(CRTMesh m : sceneObjects) {
        for(CRTVector t : m.triangleSoup) {
            std::cout << t.x << ","  << t.y << "," << t.z << std::endl;
        }
    }
}
void CRTScene::parseSceneFile(const std::string& sceneFileName){
std::ifstream ifs(sceneFileName);
    assert(ifs.is_open());
    rapidjson::IStreamWrapper isw(ifs);
    rapidjson::Document doc;
    doc.ParseStream(isw);

    CRTVector bgColor(0.f);
    int imageWidth = 0;
    int imageHeight = 0;
    assert(doc.HasMember("settings"));
    const rapidjson::Value& settingsVal = doc.FindMember("settings")->value;
    //load image settings
    if(!settingsVal.IsNull() && settingsVal.IsObject()) 
    {
        assert(settingsVal.HasMember("background_color"));
        const rapidjson::Value& bgColorVal = settingsVal.FindMember("background_color")->value;
        if(!bgColorVal.IsNull() && bgColorVal.IsArray()) {
            bgColor = loadVector(bgColorVal.GetArray());
        }
        assert(settingsVal.HasMember("image_settings") && settingsVal.FindMember("image_settings")->value.HasMember("width") && settingsVal.FindMember("image_settings")->value.HasMember("height"));
        const rapidjson::Value& widthValue = settingsVal.FindMember("image_settings")->value.FindMember("width")->value;
        const rapidjson::Value& heightValue = settingsVal.FindMember("image_settings")->value.FindMember("height")->value;
        if(!widthValue.IsNull() && widthValue.IsInt() && !heightValue.IsNull() && heightValue.IsInt()) {
            imageWidth = widthValue.GetInt();
            imageHeight = heightValue.GetInt();
        }
    }
    CRTSettings settings(bgColor,imageWidth,imageHeight);
    sceneSettings = settings;
    PPMImage image(imageWidth,imageHeight);
    image.backgroundColor = bgColor;
    sceneImage = image;
    CRTMatrix cameraRotationMatrix(CRTVector(1.f),CRTVector(1.f),CRTVector(1.f));
    CRTVector cameraPosition(0.f);
    //load camera information
    assert(doc.HasMember("camera"));
    const rapidjson::Value& cameraVal = doc.FindMember("camera")->value;
    if(!cameraVal.IsNull() && cameraVal.IsObject()) {
        assert(cameraVal.HasMember("matrix"));
        assert(cameraVal.HasMember("position"));
        const rapidjson::Value& cameraMatrixVal = cameraVal.FindMember("matrix")->value;
        if(!cameraMatrixVal.IsNull() && cameraMatrixVal.IsArray()) {
            cameraRotationMatrix = loadMatrix(cameraMatrixVal.GetArray());
        }
        const rapidjson::Value& cameraPosVal = cameraVal.FindMember("position")->value;
        if(!cameraPosVal.IsNull() && cameraPosVal.IsArray()) {
            cameraPosition = loadVector(cameraPosVal.GetArray());
        }
    }
    sceneCamera = CRTCamera(cameraPosition,cameraRotationMatrix,imageWidth,imageHeight);

    std::vector<CRTMesh> objects;
    assert(doc.HasMember("objects"));
    const rapidjson::Value& objectsVal = doc.FindMember("objects")->value;
    if(!objectsVal.IsNull() && objectsVal.IsArray()) {
        for(int i = 0; i < objectsVal.Size();i++) {
            const rapidjson::Value& objectVal = objectsVal[i];
            std::vector<int> triangleVertIndices;
            std::vector<CRTVector> triangleVertices;
            assert(objectVal.HasMember("vertices"));
            assert(objectVal.HasMember("triangles"));
            const rapidjson::Value& verticesVal = objectVal.FindMember("vertices")->value;
            if(!verticesVal.IsNull() && verticesVal.IsArray()) {
                triangleVertices = loadVertices(verticesVal.GetArray());
            }
            const rapidjson::Value& indicesVal = objectVal.FindMember("triangles")->value;
            if(!indicesVal.IsNull() && indicesVal.IsArray()) {
                triangleVertIndices = loadIndices(indicesVal.GetArray());
            }
            objects.push_back(CRTMesh(triangleVertices,triangleVertIndices));
        }
    }
    sceneObjects = objects;

    std::vector<Light> lights;
    //import light objects
    if(doc.HasMember("lights")) {
        const rapidjson::Value& lightsVal = doc.FindMember("lights")->value;
        if(!lightsVal.IsNull() && lightsVal.IsArray()) {
            for(int i = 0; i < lightsVal.Size();i++) {
                const rapidjson::Value& lightVal = lightsVal[i];
                assert(lightVal.HasMember("intensity"));
                assert(lightVal.HasMember("position"));
                const rapidjson::Value& posVal = lightVal.FindMember("position")->value;
                float intensity;
                CRTVector pos;
                if(!posVal.IsNull() && posVal.IsArray()) {
                    pos = loadVector(posVal.GetArray());
                } 
                const rapidjson::Value& intensitytVal = lightVal.FindMember("intensity")->value;
                if(!intensitytVal.IsNull() ) {
                    intensity = static_cast<float> (intensitytVal.GetDouble());
                } 
                lights.push_back(Light(pos,intensity));
            }
        }
    }
}
void CRTScene::render() {

    for(int i = 0; i < sceneSettings.imageHeight;i++) {
        for(int j = 0; j < sceneSettings.imageWidth;j++) {
            float closestIntersectionDistance = FLT_MAX;
            CRTRay ray = sceneCamera.generateCameraRay(i, j);
            bool foundIntersection = false;
            for(CRTMesh object : sceneObjects) {
                for(int k = 0; k < object.triangleVertIndices.size();k+=3) {
                    int triangleFirstIndex = object.triangleVertIndices[k];
                   
                    CRTTriangle triangle(object.triangleSoup[object.triangleVertIndices[k]],
                                        object.triangleSoup[object.triangleVertIndices[k+1]],
                                        object.triangleSoup[object.triangleVertIndices[k+2]]);

                    float t = 1.f;
                    

                    if(ray.intersectTriangle(triangle, t)) {
                        
                        if(t < closestIntersectionDistance) {
                            foundIntersection = true;
                            closestIntersectionDistance = t;
                            CRTVector color = colors[triangleFirstIndex % colors.size()];
                            sceneImage.setPixel(color, j, i);
                        }
                    }            
                }
            }
            if(!foundIntersection) sceneImage.setPixel((int)(sceneImage.backgroundColor.x *255.f), (int)(sceneImage.backgroundColor.y *255.f), (int)(sceneImage.backgroundColor.z *255.f), j, i);

        }
    }
    sceneImage.storeImageToFile("../output.ppm");
}

/*
        for(int i = 0; i < height;i++) {
            for(int j = 0; j < width;j++) {
                float lowestDistance = FLT_MAX;
                for(CRTTriangle triangle : triangles) {
                    float t = 1.f;
                    CRTRay ray = camera.generateCameraRay(i, j);
                    if(ray.intersectTriangle(triangle, t)) {
                        if(t < lowestDistance) {
                            lowestDistance = t;
                            image.setPixel(triangle.color[0], triangle.color[1], triangle.color[2], j, i);
                        }
                    } else {
                        image.setPixel((int)(image.backgroundColor.x *255.f), (int)(image.backgroundColor.y *255.f), (int)(image.backgroundColor.z *255.f), j, i);
                    }
                }
            }
        }
            */