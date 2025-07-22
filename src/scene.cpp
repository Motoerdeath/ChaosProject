#include "../headers/scene.hpp"
#include "../headers/triangle.hpp"
#include <algorithm>
#include <cassert>
#include <fstream>
#include "../include/rapidjson/document.h"
#include "../include/rapidjson/istreamwrapper.h"
#include "../headers/jsonUtilities.hpp"
#include <iostream>
#include <string>
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
            std::vector<CRTVector> textureCoords;
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
            if(objectVal.HasMember("uvs")) {
                const rapidjson::Value& uvVal = objectVal.FindMember("uvs")->value;
                assert(uvVal.IsArray());
                textureCoords = loadTextureCoordinates(uvVal.GetArray());
                objects.push_back(CRTMesh(triangleVertices,triangleVertIndices,mID,textureCoords));
            } else {
                objects.push_back(CRTMesh(triangleVertices,triangleVertIndices,mID));
            }
            
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

Material CRTScene::importBasicMaterial(rapidjson::Document& doc, int mIdx) {
    Material mat;
    CRTVector albedo(1.f);
    MaterialType matType;
    float ior =1.f;
    const rapidjson::Value& materialVal = doc.FindMember("materials")->value[mIdx];

    assert(materialVal.HasMember("type")  && materialVal.HasMember("smooth_shading"));
    RenderingStyle style;
    if(materialVal.FindMember("smooth_shading")->value.GetBool()) {
        style = smooth;
    } else {
        style = flat;
    }
    std::string typeName = materialVal.FindMember("type")->value.GetString();

    if(materialVal.HasMember("albedo")) {
        albedo = loadVector(materialVal.FindMember("albedo")->value.GetArray());
    } else {
        albedo = CRTVector(1.f);
    }

    if(!typeName.std::string::compare("refractive")) {
        if(materialVal.HasMember("ior")) {
            ior = static_cast<float>(materialVal.FindMember("ior")->value.GetDouble());
        } else {
            ior = 1.f;
            std::printf("Refractive Material without IOR value found");
        }
        
        matType = refractive;
    } else {
        if(doc.HasMember("lights") && doc.FindMember("lights")->value.IsArray() && doc.FindMember("lights")->value.Size() >0) {
            if(!typeName.std::string::compare("reflective")) {
                matType = reflective;
            } else if(!typeName.std::string::compare("diffuse")) {
                matType = diffuse;
            } else if(!typeName.std::string::compare("constant")){
                matType = constant;
            } else {
                assert(false);
            }
        } else {
            matType = constant;
        }
    }
    mat = Material(matType,albedo,style,ior);
    return mat;
}

Material CRTScene::importTextureMaterial(rapidjson::Document& doc, int mIdx) {
    Material mat;
    CRTVector albedo(1.f);
    std::string albedoTex;
    MaterialType matType;
    float ior =1.f;
    const rapidjson::Value& materialVal = doc.FindMember("materials")->value[mIdx];

    assert(materialVal.HasMember("type")  && materialVal.HasMember("smooth_shading") && materialVal.HasMember("albedo"));
    RenderingStyle style;
    if(materialVal.FindMember("smooth_shading")->value.GetBool()) {
        style = smooth;
    } else {
        style = flat;
    }
    std::string typeName = materialVal.FindMember("type")->value.GetString();

    albedoTex = materialVal.FindMember("albedo")->value.GetString();

    if(!typeName.std::string::compare("refractive")) {
        assert(materialVal.HasMember("ior"));
        ior = static_cast<float>(materialVal.FindMember("ior")->value.GetDouble());
        matType = refractive;
    } else {
        if(doc.HasMember("lights") && doc.FindMember("lights")->value.IsArray() && doc.FindMember("lights")->value.Size() >0) {
            if(!typeName.std::string::compare("reflective")) {
                matType = reflective;
            } else if(!typeName.std::string::compare("diffuse")) {
                matType = diffuse;
            } else if(!typeName.std::string::compare("constant")){
                matType = constant;
            } else {
                assert(false);
            }
        } else {
            matType = constant;
        }
    }
    mat = Material(matType,albedoTex,style,ior);
    return mat;
}
void CRTScene::importMaterials(rapidjson::Document& doc){
    
    if(doc.HasMember("materials")) {
        const rapidjson::Value& materialsVal = doc.FindMember("materials")->value;
        for(int i = 0; i < materialsVal.Size();i++) {   
            Material mat;
            const rapidjson::Value& materialVal = materialsVal[i];
            if(materialVal.HasMember("albedo")) {
                if(materialVal.FindMember("albedo")->value.IsString()) {
                    mat = importTextureMaterial(doc, i);
                } else {
                    mat = importBasicMaterial(doc, i);
                }
            } else {
                mat = importBasicMaterial(doc, i);
            }
            sceneMaterials.push_back(mat);
        }
    }
            /*
            //&& materialVal.HasMember("albedo")
            assert(materialVal.HasMember("type")  && materialVal.HasMember("smooth_shading"));
            std::string typeName = materialVal.FindMember("type")->value.GetString();

            //handle refractive/Semitransparent Materials
            if(!typeName.std::string::compare("refractive")) {
                assert(materialVal.HasMember("ior"));
                ior = static_cast<float>(materialVal.FindMember("ior")->value.GetDouble());
                matType = refractive;
            } else {
                assert(materialVal.HasMember("albedo"));
                if(materialVal.FindMember("albedo")->value.IsString()) {
                    textureAlbedo = materialVal.FindMember("albedo")->value.GetString();
                } else {
                    if(doc.HasMember("lights") && doc.FindMember("lights")->value.IsArray() && doc.FindMember("lights")->value.Size() >0) {
                        if(!typeName.std::string::compare("reflective")) {
                            matType = reflective;
                        } else if(!typeName.std::string::compare("diffuse")) {
                            matType = diffuse;
                        } else if(!typeName.std::string::compare("constant")){
                            matType = constant;
                            
                        } else {
                            assert(false);
                        }
                    } else {
                        matType = constant;
                    }
                    albedo = loadVector(materialVal.FindMember("albedo")->value.GetArray());
                }

            }

 
            RenderingStyle style;
            if(materialVal.FindMember("smooth_shading")->value.GetBool()) {
                style = smooth;
            } else {
                style = flat;
            }
            if(materialVal.FindMember("albedo")->value.IsString()) {
                mat = Material(matType,textureAlbedo,style,ior);
            } else {
                mat = Material(matType,albedo,style,ior);
            }
            
            sceneMaterials.push_back(mat);
        }
    } else {
        Material mat;
        CRTVector albedo(0.4f);
        MaterialType matType;
        RenderingStyle style = flat;
        if(doc.HasMember("lights") && doc.FindMember("lights")->value.IsArray() && doc.FindMember("lights")->value.Size() >0) {
                matType = diffuse;
        } else {
                matType = constant;
        }
        mat = Material(matType,albedo,style);
        sceneMaterials.push_back(mat);
    }
    */
    
}
void CRTScene::importMaterialsWithTextures(rapidjson::Document& doc) {
    if(doc.HasMember("materials")) {
        Material mat;
        std::string albedoTex = "Texture0";
        MaterialType matType;
        RenderingStyle style = flat;
        if(doc.HasMember("lights") && doc.FindMember("lights")->value.IsArray() && doc.FindMember("lights")->value.Size() >0) {
                matType = diffuse;
        } else {
                matType = constant;
        }
        mat = Material(matType,albedoTex,style,1.f);
        sceneMaterials.push_back(mat);
    } else {
        Material mat;
        std::string albedoTex = "Texture0";
        MaterialType matType;
        RenderingStyle style = flat;
        if(doc.HasMember("lights") && doc.FindMember("lights")->value.IsArray() && doc.FindMember("lights")->value.Size() >0) {
                matType = diffuse;
        } else {
                matType = constant;
        }
        mat = Material(matType,albedoTex,style,1.f);
        sceneMaterials.push_back(mat);
    }
}

void CRTScene::importTextures(rapidjson::Document& doc) {
    if(doc.HasMember("textures") ) {
        const rapidjson::Value& texturesVal = doc.FindMember("textures")->value;
        assert(texturesVal.IsArray());
        for(int i = 0; i < texturesVal.Size(); i++) {
            const rapidjson::Value& textureVal = texturesVal[i];
            assert(textureVal.HasMember("type") && textureVal.HasMember("name"));
            std::string typeName = textureVal.FindMember("type")->value.GetString();
            std::string textureName = textureVal.FindMember("name")->value.GetString();
            Texture newTexture{};
            newTexture.name = textureName;
            if(!typeName.std::string::compare("albedo")) {
                //load albedo texture
                newTexture.type = albedoTexture;
                newTexture.albedo = loadVector(textureVal.FindMember("albedo")->value.GetArray());
            } else if(!typeName.std::string::compare("edges")) {
                assert(textureVal.HasMember("inner_color") &&textureVal.HasMember("edge_color") && textureVal.HasMember("edge_width"));
                newTexture.type = edgeTexture;
                newTexture.innerColor = loadVector(textureVal.FindMember("inner_color")->value.GetArray());
                newTexture.edgeColor = loadVector(textureVal.FindMember("edge_color")->value.GetArray());
                newTexture.edgeWidth = static_cast<float>(textureVal.FindMember("edge_width")->value.GetDouble());
                //load edges Texture
            } else if(!typeName.std::string::compare("checker")) {
                assert(textureVal.HasMember("color_A") &&textureVal.HasMember("color_B") && textureVal.HasMember("square_size"));
                newTexture.type = checkersTexture;
                newTexture.colorA = loadVector(textureVal.FindMember("color_A")->value.GetArray());
                newTexture.colorB = loadVector(textureVal.FindMember("color_B")->value.GetArray());
                newTexture.squareSize = static_cast<float>(textureVal.FindMember("square_size")->value.GetDouble());

            } else if (!typeName.std::string::compare("bitmap")) {
                assert(textureVal.HasMember("file_path"));
                newTexture.type = bitmapTexture;
                std::string textureFilePath = textureVal.FindMember("file_path")->value.GetString();
                int width, height;
                newTexture.buffer = loadImageFromFile("../inputs/Homework12_Textures"+ textureFilePath,width,height);
                newTexture.bitmapHeight = height;
                newTexture.bitmapWidth = width;

            } else {
                std::cout << "unsupported texture type encountered" << std::endl;
                assert(false);
            }
            sceneTextures.push_back(newTexture);
        }
    } else {
        if(doc.HasMember("materials")) {
            const rapidjson::Value& materialsVal = doc.FindMember("materials")->value;
            for(int i = 0; i < materialsVal.Size();i++) {
                const rapidjson::Value& materialVal = materialsVal[i];
                Texture newTexture;
                newTexture.name = "Texture"+ std::to_string(i);
                if(!materialVal.HasMember("albedo")) {
                    newTexture.type = invalidTexture;
                } else {
                    newTexture.type = albedoTexture;
                    newTexture.albedo = loadVector(materialVal.FindMember("albedo")->value.GetArray());
                }
                sceneTextures.push_back(newTexture);
            }
        } else {
            Texture newTexture;
            newTexture.name = "Texture0";
            newTexture.albedo = CRTVector(0.4f);
            newTexture.type = albedoTexture;
            sceneTextures.push_back(newTexture);
        }
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
    
    importTextures(doc);
    std::cout << "j" << std::endl;
    //textureImage = loadImageFromFile("../inputs/Homework12_Textures/textures/dragon.jpg");
}

Material CRTScene::getMaterial(int materialIDx) {
    return sceneMaterials[materialIDx];
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


void CRTScene::parseSceneFile2(const std::string& sceneFileName) {


    //import settings
        //import backgroundColor: CRTVector
        //import ImageSettings:
            //import width : int
            //import height : int
            //import bucketsize : int
    //import Camera
        //import RotationMatrix : 3x3 Matrix
        //import Position : CRTVector
    //import lights : Array
        //for each light:
            //import intensity : int
            //import position : CRTVector
    //import Materials: Array
        //for each material
            //import Type : string valid forms: "diffuse"/"reflective"/"refractive"/"constant"
            //if "back_face_culling" is present, set that value otherwise set it to false
            //import Albedo if pre
    

}