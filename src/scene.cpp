#include "../headers/scene.hpp"
#include "../headers/triangle.hpp"
#include"rapidjson/rapidjson.h"
#include <algorithm>
#include <fstream>
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "../headers/jsonUtilities.hpp"
#include <iostream>

void CRTScene::parse() {
    for(CRTMesh m : sceneObjects) {
        for(CRTVector t : m.triangleVertices) {
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
    sceneLights = lights;
}



void CRTScene::traceRay() {

}
void CRTScene::traceShadowRay() {
    
}

bool CRTScene::isShadowed(CRTVector pos, CRTVector lightDir) {
    CRTRay shadowRay(pos,lightDir);

            for(CRTMesh object : sceneObjects) {
                for(int k = 0; k < object.triangleVertIndices.size();k+=3) {
                    int triangleFirstIndex = object.triangleVertIndices[k];
                    CRTTriangle triangle(object.triangleVertices[object.triangleVertIndices[k]],
                                        object.triangleVertices[object.triangleVertIndices[k+1]],
                                        object.triangleVertices[object.triangleVertIndices[k+2]]);
                    float t = 1.f;
                    if(shadowRay.intersectTriangle(triangle, t,true)) {
                        
                        return true;
                    }     
                }
            }  
    return false;
}
CRTVector CRTScene::shade(CRTVector pos,CRTVector triangleNormal) {
    CRTVector color(0.f);
    CRTVector albedo = CRTVector(0.4f);
    //remove shadowacne by offsetting position a small amount in the direction of the normal
    CRTVector adjPos = pos + triangleNormal*0.00001f;
    for(Light source : sceneLights) {
        //determine vector to light source from intersectionPoint
        CRTVector lD = (source.lightPosition - adjPos);
        if(isShadowed(adjPos, lD.normalize())) continue;
        float lDLength = lD.length();
        //determine if surface is oriented towards light
        float cosLaw = std::max(0.f,CRTVector::dot(lD.normalize(), triangleNormal));
        if(cosLaw ==0.f) continue;
        float distanceFallOff = 4*M_PI*lDLength*lDLength;
        CRTVector temp = color +(albedo*(cosLaw*source.lightIntensity/distanceFallOff));
        color = CRTVector(glm::clamp(temp.x,0.f,1.f),glm::clamp(temp.y,0.f,1.f),glm::clamp(temp.y,0.f,1.f));
        //color = CRTVector(cosLaw);
        //color = CRTVector(lDLength);
    }
    return color;
}
void CRTScene::render() {

    //iterate over all pixels
    for(int i = 0; i < sceneSettings.imageHeight;i++) {
        for(int j = 0; j < sceneSettings.imageWidth;j++) {

            //setup
            float closestIntersectionDistance = FLT_MAX;
            CRTRay ray = sceneCamera.generateCameraRay(i, j);
            bool foundIntersection = false;
            int intersectedTriangleIndex;
            CRTTriangle intersectedTriangle;
            CRTVector intersectionPoint;

            //trace the ray through the scene to determine closest intersected Triangle
            for(CRTMesh object : sceneObjects) {
                for(int k = 0; k < object.triangleVertIndices.size();k+=3) {
                    int triangleFirstIndex = object.triangleVertIndices[k];
                    CRTTriangle triangle(object.triangleVertices[object.triangleVertIndices[k]],
                                        object.triangleVertices[object.triangleVertIndices[k+1]],
                                        object.triangleVertices[object.triangleVertIndices[k+2]]);
                    float t = 1.f;
                    if(ray.intersectTriangle(triangle, t,false)) {
                        
                        if(t < closestIntersectionDistance) {
                            foundIntersection = true;
                            closestIntersectionDistance = t;
                            intersectedTriangleIndex = k;
                            intersectionPoint = ray.rayOrigin + ray.rayDirection*t;
                            intersectedTriangle = triangle;
                        }
                    }     
                }
            }  
            
            //perform shading
            //CRTVector color = colors[intersectedTriangleIndex % colors.size()];
            CRTVector color;
            if(foundIntersection) {
                color= shade(intersectionPoint, intersectedTriangle.normal);
                //color = CRTVector(1.f);
            } else {
                color = sceneImage.backgroundColor;
                //sceneImage.setPixel((int)(sceneImage.backgroundColor.x *255.f), (int)(sceneImage.backgroundColor.y *255.f), (int)(sceneImage.backgroundColor.z *255.f), j, i);
            }
            sceneImage.setPixel(color, j, i);
        }
    }
    sceneImage.storeImageToFile("../output.ppm");
}
