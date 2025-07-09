#include "../headers/scene.hpp"
#include "../headers/triangle.hpp"
#include <algorithm>
#include <fstream>
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "../headers/jsonUtilities.hpp"
#include <iostream>
#include <vector>

void CRTScene::parse() {
    for(CRTMesh m : sceneObjects) {
        std::cout << m.vertexNormals.size() << std::endl;
        std::cout << m.triangleVertices.size() << std::endl;
        for(CRTVector t : m.triangleVertices) {
            //std::cout << t.x << ","  << t.y << "," << t.z << std::endl;
        }
    }
}

void CRTScene::importSettings(rapidjson::Document& doc){

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

}
void CRTScene::importCamera(rapidjson::Document& doc, int width, int height){

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
    sceneCamera = CRTCamera(cameraPosition,cameraRotationMatrix,width,height);
}
void CRTScene::importObjects(rapidjson::Document& doc){

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
            int mID = 0;
            if(objectVal.HasMember("material_index")) {
                const rapidjson::Value& mIDVal = objectVal.FindMember("material_index")->value;
                assert(mIDVal.IsInt());
                mID = mIDVal.GetInt();
            }
            objects.push_back(CRTMesh(triangleVertices,triangleVertIndices,mID));
        }
    }
    sceneObjects = objects;

}
void CRTScene::importLights(rapidjson::Document& doc){

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
void CRTScene::importMaterials(rapidjson::Document& doc){
    
    if(doc.HasMember("materials")) {
        const rapidjson::Value& materialsVal = doc.FindMember("materials")->value;
        for(int i = 0; i < materialsVal.Size();i++) {
            Material mat;
            const rapidjson::Value& materialVal = materialsVal[i];
            assert(materialVal.HasMember("type") && materialVal.HasMember("albedo") && materialVal.HasMember("smooth_shading"));
            std::string temp = materialVal.FindMember("type")->value.GetString();
            MaterialType matType = diffuse;
            
            if(!temp.std::string::compare("reflective")) {
                matType = reflective;
            } else {
                matType = diffuse;
            }
                
            RenderingStyle style;
            if(materialVal.FindMember("smooth_shading")->value.GetBool()) {
                style = smooth;
            } else {
                if(doc.HasMember("lights") && doc.FindMember("lights")->value.IsArray() && doc.FindMember("lights")->value.Size() >0) {
                    style = flat;
                } else {
                    style = constant;
                }
            }
            CRTVector albedo = loadVector(materialVal.FindMember("albedo")->value.GetArray());
            mat = Material(matType,albedo,style);
            sceneMaterials.push_back(mat);
        }
    } else {
        Material mat;
        CRTVector albedo(0.1f);
        MaterialType matType = diffuse;
        RenderingStyle style;
        if(doc.HasMember("lights") && doc.FindMember("lights")->value.IsArray() && doc.FindMember("lights")->value.Size() >0) {
                style = flat;
        } else {
                style = constant;
        }
        mat = Material(matType,albedo,style);
        sceneMaterials.push_back(mat);
    }
    
}

void CRTScene::parseSceneFile(const std::string& sceneFileName){
std::ifstream ifs(sceneFileName);
    assert(ifs.is_open());
    rapidjson::IStreamWrapper isw(ifs);
    rapidjson::Document doc;
    doc.ParseStream(isw);

    importSettings(doc);
    importCamera(doc, sceneSettings.imageWidth, sceneSettings.imageHeight);

    importObjects(doc);

    importLights(doc);

    importMaterials(doc);
    //import materials
}



bool CRTScene::traceRay(CRTRay& ray, Intersection& isect) {


    float closestIntersectionDistance = FLT_MAX;
    bool foundIntersection = false;
    int intersectedTriangleIndex;
    CRTTriangle intersectedTriangle;
    CRTVector intersectionPoint;
    CRTVector intersectionPoint_bary;
    CRTVector smoothedNormal1;
    CRTVector final_point;
    CRTMesh* isecObject;
    int materialID = 0;
    int objectIndex;
    int p = 0;
    std::vector<float> bary;
    CRTVector baryCoords;
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
                    isecObject = &object;
                    objectIndex = p;
                    materialID = object.materialID;
                    baryCoords = CRTTriangle::calculateBarycentricCoordinates(intersectedTriangle, intersectionPoint);
                    smoothedNormal1 = object.vertexNormals[object.triangleVertIndices[k]]*baryCoords.z +object.vertexNormals[object.triangleVertIndices[k+1]]*baryCoords.x + object.vertexNormals[object.triangleVertIndices[k+2]]*baryCoords.y;
                    intersectionPoint_bary = object.triangleVertices[object.triangleVertIndices[k]]*baryCoords.z +object.triangleVertices[object.triangleVertIndices[k+1]]*baryCoords.x + object.triangleVertices[object.triangleVertIndices[k+2]]*baryCoords.y;
                    intersectionPoint = intersectionPoint + triangle.normal*0.1f;
                    final_point = intersectionPoint_bary + triangle.normal*0.1f;
                    intersectionPoint_bary = CRTRay::offsetRay(intersectionPoint_bary, triangle.normal);
                    //final_point = CRTRay::offsetRay(intersectionPoint_bary, triangle.normal);
                }
            }     
        }
        p++;
    }
    if(!foundIntersection) return false;
    isect.t = closestIntersectionDistance;
    isect.intersectedObject = isecObject;
    isect.intersectionPoint = final_point;
    isect.intersectionTriangle = intersectedTriangle;
    isect.baryCoords = baryCoords;//CRTVector(bary[0],bary[1],1.f-bary[0]-bary[1]);
    isect.shadingNormal = smoothedNormal1;
    isect.mID = materialID;
    return true;
}
void CRTScene::traceShadowRay(CRTRay& ray) {
    
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

CRTVector CRTScene::constantShade(Intersection& isect) {
    return sceneMaterials[isect.mID].albedo;
}
CRTVector CRTScene::flatShade(Intersection& isect) {
    CRTVector color(0.f);
    CRTVector albedo = sceneMaterials[isect.mID].albedo;
    //remove shadowacne by offsetting position a small amount in the direction of the normal
    CRTVector adjPos = isect.intersectionPoint + isect.intersectionTriangle.normal*0.01f;
    for(Light source : sceneLights) {
        //determine vector to light source from intersectionPoint
        CRTVector lD = (source.lightPosition - adjPos);
        if(isShadowed(adjPos, lD.normalize())) continue;
        float lDLength = lD.length();
        //determine if surface is oriented towards light
        float cosLaw = std::max(0.f,CRTVector::dot(lD.normalize(), isect.intersectionTriangle.normal));
        if(cosLaw ==0.f) continue;
        float distanceFallOff = 4*M_PI*lDLength*lDLength;
        CRTVector temp = color +(albedo*(cosLaw*source.lightIntensity/distanceFallOff));
        color = CRTVector(glm::clamp(temp.x,0.f,1.f),glm::clamp(temp.y,0.f,1.f),glm::clamp(temp.z,0.f,1.f));
        //color = CRTVector(cosLaw);
        //color = CRTVector(lDLength);
    }
    return color;
}

CRTVector CRTScene::shade(Intersection& isect) {
    Material  mat = sceneMaterials[isect.mID];
    CRTVector color(0.f);
    //CRTVector albedo = CRTVector(0.4f);
    CRTVector albedo = mat.albedo;
    //remove shadowacne by offsetting position a small amount in the direction of the normal
    //CRTVector adjPos = pos + triangleNormal*0.1f;
    for(Light source : sceneLights) {
        //determine vector to light source from intersectionPoint
        CRTVector lD = (source.lightPosition - isect.intersectionPoint);
        if(isShadowed(isect.intersectionPoint, lD.normalize())) continue;
        float lDLength = lD.length();
        //determine if surface is oriented towards light
        float cosLaw = std::max(0.f,CRTVector::dot(lD.normalize(), isect.shadingNormal));
        if(cosLaw ==0.f) continue;
        float distanceFallOff = 4*M_PI*lDLength*lDLength;
        CRTVector temp = color +(albedo*(cosLaw*source.lightIntensity/distanceFallOff));
        color = CRTVector(glm::clamp(temp.x,0.f,1.f),glm::clamp(temp.y,0.f,1.f),glm::clamp(temp.z,0.f,1.f));
        //color = CRTVector(cosLaw);
        //color = CRTVector(lDLength);
    }
    return color;
}

//CRTVector determineHitLocation(CRTVector rayOrigin, CRTVector rayDirection, float rayDistance);
void CRTScene::render() {

    int maxDepth =4;
    //iterate over all pixels
    for(int i = 0; i < sceneSettings.imageHeight;i++) {
        for(int j = 0; j < sceneSettings.imageWidth;j++) {
            CRTVector color;
            CRTVector contrib(1.f);
            CRTRay ray = sceneCamera.generateCameraRay(i, j);
            for(int depth = 0; depth < maxDepth;depth++) {
                if(depth == 3) {color = sceneImage.backgroundColor;break;}
                Intersection isect;
                if(traceRay(ray, isect)) {
                    if(sceneMaterials[isect.mID].type == diffuse) {
                        Material mat = sceneMaterials[isect.mID];
                        RenderingStyle style = mat.style;
                        if(style == constant) {
                            color = constantShade(isect);
                            color = CRTVector(color.x*contrib.x,color.y*contrib.y,color.z*contrib.z); 
                        } else if(style == flat) {
                            color = flatShade(isect);
                            color = CRTVector(color.x*contrib.x,color.y*contrib.y,color.z*contrib.z);
                        } else {
                            color= shade(isect);
                            color = CRTVector(color.x*contrib.x,color.y*contrib.y,color.z*contrib.z);
                            //color= mat.albedo;//shade(isect);
                        }
                        break;
                    } else {
                        Material mat = sceneMaterials[isect.mID];
                        ray = CRTRay(isect.intersectionPoint,CRTRay::reflect(ray.rayDirection, isect.shadingNormal));
                        contrib= CRTVector(contrib.x*mat.albedo.x,contrib.y*mat.albedo.y, contrib.z*mat.albedo.z);
                    }
                    
                } else {
                    color = sceneImage.backgroundColor;
                    color = CRTVector(color.x*contrib.x,color.y*contrib.y,color.z*contrib.z);
                    break;
                }
            }

            sceneImage.setPixel(color, j, i);
        }
    }
}


const CRTVector CRTScene::getBackgroundColor() {
    return sceneImage.backgroundColor;
}

CRTSettings* CRTScene::getSettings() {
    return &sceneSettings;
}
CRTCamera* CRTScene::getCamera() {
    return &sceneCamera;
}