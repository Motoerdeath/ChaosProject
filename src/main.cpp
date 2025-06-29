#include "../src/Tasks/homework2.cpp"
#include "../src/Tasks/homework3.cpp"
#include "../src/Tasks/homework4.cpp"
#include "../src/Tasks/homework5.cpp"

#include "../headers/triangle.hpp"
#include "../headers/camera.hpp"
#include "glm/geometric.hpp"
#include <cfloat>
#include <cstdint>
#include "../headers/matrix.hpp"

bool FUNDAMENTALS = true;
bool RAYS = true;



int main() {
    
    Homework2 task1;
    task1.execute();

    Homework3 task2;
    task2.execute();

    Homework4 task3;
    task3.execute();

    Homework5 task4;
    task4.execute();
    

    /*
    CRTTriangle triangle1(CRTVector(-1.75f,-1.75f,-3.f),CRTVector(1.75f,-1.75f,-3.f),CRTVector(0.f,1.75f,-3.f));
    triangle1.color = {0,255,0};
    std::vector<CRTTriangle> triangles;
    triangles.push_back(triangle1);
    
    
    CRTTriangle triangle2(CRTVector(-2.75f,-1.75f,-2.f),CRTVector(.75f,-.75f,-4.f),CRTVector(1.5f,2.25f,-4.f));
    triangle2.color = {255,0,0};
    //CRTTriangle triangle(CRTVector(-1.75f,-1.75f,-3.f),CRTVector(1.75f,-1.75f,-3.f),CRTVector(0.f,1.75f,-3.f));
    
    //CRTTriangle tri3(CRTVector(-2.f,-2.f,-3.f),CRTVector(-3.f,1.f,-3.f),CRTVector(-1.f,0.f,-3.f));
    
    
    //triangles.push_back(triangle);
    //triangles.push_back(triangle2);

    std::vector<CRTTriangle> triangles2;
    CRTTriangle tri1(CRTVector(-1.75f,-1.75f,-3.f),CRTVector(1.75f,-1.75f,-3.f),CRTVector(0.f,1.75f,-3.f));
    CRTTriangle tri5(CRTVector(0.f,0.0f,-3.f),CRTVector(1.f,0.f,-3.f),CRTVector(0.f,1.f,-3.f));


    CRTTriangle tri2(CRTVector(-1.f,0.5f,-2.f),CRTVector(2.f,0.5f,-3.f),CRTVector(-1.f,1.5f,-2.5f));
    CRTTriangle tri3(CRTVector(2.f,0.5f,-3.f),CRTVector(-4.f,0.5f,-3.f),CRTVector(-1.f,1.5f,-2.5f));
    CRTTriangle tri4(CRTVector(-1.f,0.5f,-2.f),CRTVector(-4.f,0.5f,-3.f),CRTVector(2.f,0.5f,-3.f));
    tri1.color = {255,0,0};
    tri5.color = {0,255,0};
    tri3.color = {0,0,255};
    tri4.color = {255,255,255};

    triangles2.push_back(triangle1);
    //triangles2.push_back(tri5);
    //triangles2.push_back(triangle1);
    //triangles2.push_back(tri2);
    //triangles2.push_back(tri3);
    //triangles2.push_back(tri4);
    std::vector<int> triangleColor = {0,255,0};
    
    int width = 1920;
    int height = 1080;
    PPMImage image(width, height);
    //image.configureGrid(4, 4);
    //image.renderRandomizedBackground();
    image.generateCameraRays(CRTVector(0.f));
    for(int i = 0; i < height;i++) {
        for(int j = 0; j < width;j++) {
            float lowestDistance = FLT_MAX;
            for(CRTTriangle triangle : triangles2) {
                float t = 1.f;
                if(image.cameraRays[i][j].intersectTriangle(triangle, t)) {
                    if(t < lowestDistance) {
                        lowestDistance = t;
                        image.setPixel(triangle.color[0], triangle.color[1], triangle.color[2], j, i);
                    }
                }
            }

        }
    }
    image.storeImageToFile("../outputs/05_Triangle/output.ppm");
    */
    return 0;
}