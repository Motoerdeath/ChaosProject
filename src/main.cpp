#include "../src/Tasks/homework2.cpp"
#include "../src/Tasks/homework3.cpp"
#include "../src/Tasks/homework4.cpp"
#include "../src/Tasks/homework5.cpp"

#include "../headers/triangle.hpp"
#include "../headers/camera.hpp"
#include "../headers/settings.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include <cassert>
#include <cfloat>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <string>
#include <vector>
#include "../headers/matrix.hpp"
#include "rapidjson/rapidjson.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/document.h"
#include <iostream>
//#include "../src/jsonUtilities.cpp"
#include "../headers/mesh.hpp"
#include "../headers/scene.hpp"

bool FUNDAMENTALS = true;
bool RAYS = true;



int main() {
  /*  
    Homework2 homework2;
    homework2.execute();

    Homework3 homework3;
    homework3.execute();

    Homework4 homework4;
    homework4.execute();

    Homework5 homework5;
    homework5.execute();
 */
    //Homework6 Task1
/*
    CRTVector a(1.f,1.f,1.f);
    CRTMatrix rotMatrix(CRTVector(1.f,0.f,0.f),CRTVector(0.f,1.f,0.f),CRTVector(0.f,0.f,1.f));
    CRTMatrix rMatrix = CRTMatrix::getRotationMatrixAroundY(20.f);
    rotMatrix = rotMatrix* rMatrix;
    CRTVector b = rotMatrix * a;
    CRTVector c = rotMatrix * CRTVector(1.f,0.f,0.f);
    CRTVector d = b + c;

    a+b;
    
    CRTVector vector(0.f,0.f,-1.f);
    vector = vector * CRTMatrix::getRotationMatrixAroundY(30.f);

    //homework6 task2
    int height = 1080;
    int width = 1920;
    PPMImage image(1920,1080);
    CRTCamera camera(CRTVector(0.f,0.0f,0.0f),CRTVector(0.f,0.f,-1.f),1920,1080);
    CRTTriangle triangle1(CRTVector(-1.75f,-1.75f,-3.f),CRTVector(1.75f,-1.75f,-3.f),CRTVector(0.f,1.75f,-3.f));
    triangle1.color = {255,0,0};
    std::vector<CRTTriangle> triangles = {triangle1};

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
                }
            }

        }
    }
    image.storeImageToFile("../outputs/06_Camera/Task2.ppm");
    //Homework6 task5 
    std::vector<CRTTriangle> cubeTriangles;
    CRTTriangle cubeTriangleFront1(CRTVector(-1.f,-1.f,-3.f),CRTVector(1.f,-1.f,-3.f),CRTVector(-1.f,1.f,-3.f));
    cubeTriangleFront1.color = {255,255,255};
    CRTTriangle cubeTriangleFront2(CRTVector(1.f,-1.f,-3.f),CRTVector(1.f,1.f,-3.f),CRTVector(-1.f,1.f,-3.f));
    cubeTriangleFront2.color = {255,255,0};
    CRTTriangle cubeTriangleTop1(CRTVector(-1.f,1.f,-3.f),CRTVector(1.f,1.f,-3.f),CRTVector(-1.f,1.f,-5.f));
    cubeTriangleTop1.color = {255,0,0};
    CRTTriangle cubeTriangleTop2(CRTVector(1.f,1.f,-3.f),CRTVector(1.f,1.f,-5.f),CRTVector(-1.f,1.f,-5.f));
    cubeTriangleTop2.color = {255,0,255};

    CRTTriangle cubeTriangleBack1(CRTVector(1.f,-1.f,-5.f),CRTVector(-1.f,-1.f,-5.f),CRTVector(1.f,1.f,-5.f));
    cubeTriangleBack1.color = {0,0,255};
    CRTTriangle cubeTriangleBack2(CRTVector(-1.f,-1.f,-5.f),CRTVector(-1.f,1.f,-5.f),CRTVector(1.f,1.f,-5.f));
    cubeTriangleBack2.color = {0,255,0};

    CRTTriangle cubeTriangleLeft1(CRTVector(-1.f,-1.f,-5.f),CRTVector(-1.f,-1.f,-3.f),CRTVector(-1.f,1.f,-5.f));
    cubeTriangleLeft1.color = {0,255,255};
    CRTTriangle cubeTriangleLeft2(CRTVector(-1.f,-1.f,-3.f),CRTVector(-1.f,1.f,-3.f),CRTVector(-1.f,1.f,-5.f));
    cubeTriangleLeft2.color = {128,0,0};

    CRTTriangle cubeTriangleRight1(CRTVector(1.f,-1.f,-3.f),CRTVector(1.f,-1.f,-5.f),CRTVector(1.f,1.f,-3.f));
    cubeTriangleRight1.color = {0,128,0};
    CRTTriangle cubeTriangleRight2(CRTVector(1.f,-1.f,-5.f),CRTVector(1.f,1.f,-5.f),CRTVector(1.f,1.f,-3.f));
    cubeTriangleRight2.color = {0,0,128};

    CRTTriangle cubeTriangleBot1(CRTVector(-1.f,-1.f,-5.f),CRTVector(1.f,-1.f,-5.f),CRTVector(-1.f,-1.f,-3.f));
    cubeTriangleBot1.color = {128,0,128};
    CRTTriangle cubeTriangleBot2(CRTVector(1.f,-1.f,-5.f),CRTVector(1.f,-1.f,-3.f),CRTVector(-1.f,-1.f,-3.f));
    cubeTriangleBot2.color = {128,128,0};

    cubeTriangles = {cubeTriangleFront1,cubeTriangleFront2,cubeTriangleTop1,cubeTriangleTop2,
                    cubeTriangleBack1,cubeTriangleBack2,cubeTriangleLeft1,cubeTriangleLeft2,
                    cubeTriangleRight1, cubeTriangleRight2,cubeTriangleBot1,cubeTriangleBot2};
    int numFrames = 60;
    camera.roll(20.f);
    for(int i = 0; i < numFrames;i++) {


        image.resetImage();
        CRTVector c  =CRTVector(0.f,0.5f,0.f);
  
        for(int i = 0; i < height;i++) {
            for(int j = 0; j < width;j++) {
                float lowestDistance = FLT_MAX;
                for(CRTTriangle triangle : cubeTriangles) {
                    float t = 1.f;
                    CRTRay ray = camera.generateCameraRay(i, j);
                    if(ray.intersectTriangle(triangle, t)) {
                        if(t < lowestDistance) {
                            lowestDistance = t;
                            image.setPixel(triangle.color[0], triangle.color[1], triangle.color[2], j, i);
                        }
                    }
                }

            }
        }
        
        camera.move(c);
        //camera.pan(-5.f);
        //camera.lookAt(CRTVector(0.f,0.f,-4.f));  
        std::string filestring = "../outputs/06_Camera/Task5/Frame" + std::to_string(i)+ ".ppm";
        image.storeImageToFile(filestring);

    }
*/
    
    
    const std::string filename = "../inputs/Homework7_Scene/scene4.crtscene";


CRTScene scene(filename);
scene.parseSceneFile(filename);
scene.render();

/*

        const rapidjson::Value::ConstArray objectsArr = objectsVal.GetArray();
        for(int i = 0; i < objectsArr.Size();i++) {
            const rapidjson::Value object = objectsArr[i].GetObject();
            std::vector<int> triangleVertIndices;
            std::vector<CRTVector> triangleVertices;
            assert(object.HasMember("vertices") );
            const rapidjson::Value& verticesVal = object.FindMember("vertices")->value;
            if(!verticesVal.IsNull() && verticesVal.IsArray()) {
                triangleVertices = loadVertices(verticesVal.GetArray());
            }
            const rapidjson::Value& indicesVal = object.FindMember("triangles")->value;
            if(!indicesVal.IsNull() && indicesVal.IsArray()) {
                triangleVertIndices = loadIndices(indicesVal.GetArray());
            }
            objects.push_back(CRTMesh(triangleVertices,triangleVertIndices));
        }
    */
    std::vector<int> test;
    //load scene objects information
    /*/
    assert(doc.HasMember("camera"));
    const rapidjson::Value& cameraVal = doc.FindMember("camera")->value;
    if(!cameraVal.IsNull() && cameraVal.IsObject()) {
        assert(doc.HasMember("matrix"));
        assert(doc.HasMember("position"));
        const rapidjson::Value& cameraMatrixVal = cameraVal.FindMember("matrix")->value;
        if(!cameraMatrixVal.IsNull() && cameraMatrixVal.IsArray()) {
            cameraRotationMatrix = loadMatrix(cameraMatrixVal.GetArray());
        }
        const rapidjson::Value& cameraPosVal = cameraVal.FindMember("position")->value;
        if(!cameraPosVal.IsNull() && cameraPosVal.IsArray()) {
            cameraPosition = loadVector(cameraPosVal.GetArray());
        }
    }
        */
    return 0;
}