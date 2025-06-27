#include "../src/Tasks/task1.cpp"
#include "../src/Tasks/task2.cpp"
#include "../src/Tasks/task3.cpp"

#include "../headers/triangle.hpp"
#include "../headers/camera.hpp"
#include "glm/geometric.hpp"
#include <cfloat>
#include <cstdint>

bool FUNDAMENTALS = true;
bool RAYS = true;



int main() {

    Task1 task1;
    task1.execute();

    Task2 task2;
    task2.execute();

    Task3 task3;
    task3.execute();

    CRTTriangle triangle(CRTVector(-1.75f,-1.75f,-3.f),CRTVector(1.75f,-1.75f,-3.f),CRTVector(0.f,1.75f,-3.f));
    triangle.color = {0,255,0};
    CRTTriangle triangle2(CRTVector(-2.75f,-1.75f,-2.f),CRTVector(.75f,-.75f,-4.f),CRTVector(1.5f,2.25f,-4.f));
    triangle2.color = {255,0,0};
    //CRTTriangle triangle(CRTVector(-1.75f,-1.75f,-3.f),CRTVector(1.75f,-1.75f,-3.f),CRTVector(0.f,1.75f,-3.f));
    CRTVector cameraPosition(0.f,0.f,0.f);
    std::vector<CRTTriangle> triangles;
    triangles.push_back(triangle);
    triangles.push_back(triangle2);

    std::vector<int> triangleColor = {0,255,0};
    int width = 1920;
    int height = 1080;
    PPMImage image(width, height);
    //image.configureGrid(4, 4);
    //image.renderRandomizedBackground();
    image.generateCameraRays();
    for(int i = 0; i < height;i++) {
        for(int j = 0; j < width;j++) {
            float lowestDistance = FLT_MAX;
            for(CRTTriangle triangle : triangles) {
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
    image.storeImageToFile("../outputs/05_Triangle2/output.ppm");
    return 0;
}