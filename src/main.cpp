#include "../src/Tasks/task1.cpp"
#include "../src/Tasks/task2.cpp"
#include "../src/Tasks/task3.cpp"

#include "../headers/triangle.hpp"
#include "../headers/camera.hpp"
#include "glm/geometric.hpp"

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
    CRTVector cameraPosition(0.f,0.f,0.f);

    std::vector<int> triangleColor = {0,255,0};
    int width = 1920;
    int height = 1080;
    PPMImage image(width, height);
    image.configureGrid(4, 4);
    image.renderRandomizedBackground();
    image.generateCameraRays();
    for(int i = 0; i < height;i++) {
        for(int j = 0; j < width;j++) {
            if(image.cameraRays[i][j].intersectTriangle(triangle)) {
                image.setPixel(triangleColor[0], triangleColor[1], triangleColor[2], j, i);
            } else {
                //image.setPixel(0, 0, 0, j, i);
            }
        }
    }
    image.storeImageToFile("../outputs/05_Triangle2/output.ppm");
/*
    for(int i = 0; i < height;i++) {
        for(int j = 0; j < width;j++) {

        float x = (float) j +0.5f;
        float y = (float) (height-i) -0.5f;

        x /= width;
        y /= height;
        x = (2.f*x)-1.0f;
        y = 1.f - (2.f*y);
        x *= ((float) width / (float) height);

        CRTVector rayDir2(x,y,-1.f);
        CRTVector normalizedRayDir = rayDir2.normalize();
        CRTVector o(0.f);
        CRTRay newRay(o, normalizedRayDir);
        newRay.rayOrigin = o;
        newRay.rayDirection = normalizedRayDir;
        pixelRays[i][j] = newRay;
        }
    }


    
    //perform triangle intersection test
    for(int i = 0; i < height;i++) {
        for(int j = 0; j < width;j++) {

            if(pixelRays[i][j].intersectTriangle(triangle)) {
                image.setPixel(255, 255, 255, j, i);
            } else {
                image.setPixel(0, 0, 0, j, i);
            }
        }
    }
        
    
*/

    return 0;
}