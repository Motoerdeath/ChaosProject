#include "../headers/homework.hpp"
#include "../headers/ppmImage.hpp"
#include <time.h>
#include <random>
#include <string>
class Homework5 : public Homework {
    public:
        Homework5() {setup();};
        void execute() override {
            run();
            cleanup();
        }
    private:
    std::string resultFilePath = "../outputs/05_Triangle/";
    PPMImage image;
    CRTVector cameraPosition = CRTVector(0.f);
    int height = 1080;
    int width = 1920;
    std::vector<CRTTriangle> triangles = {};

    void renderTriangles(std::vector<CRTTriangle> triangles, PPMImage &image) {
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
    }

    void setup() override {

        image = PPMImage(width,height);
    }
    void run() override {  

        //Task1: render one triangle with specific coordinates
        CRTTriangle triangle1(CRTVector(-1.75f,-1.75f,-3.f),CRTVector(1.75f,-1.75f,-3.f),CRTVector(0.f,1.75f,-3.f));
        triangle1.color = {255,255,255};
        triangles.push_back(triangle1);

        image.generateCameraRays(cameraPosition);
        renderTriangles(triangles,image);
        image.storeImageToFile(resultFilePath+"Task1.ppm");

        //render one triangle with self chosen triangles
        //since the camera has not been movedd we can reuse the Camera rays
        triangles.clear();
        image.resetImage();
        CRTTriangle triangle2(CRTVector(0.0f,-1.f,-2.f),CRTVector(3.f,-2.f,-2.f),CRTVector(3.f,2.f,-2.f));

        triangle2.color = {255,0,0};
        triangles.push_back(triangle2);
        renderTriangles(triangles,image);
        image.storeImageToFile(resultFilePath+"Task2.ppm");

        //render 2 triangles, make them overlap
        triangles.clear();
        image.resetImage();
        //add triangles in inverse order to their actual order in scene to guarantee depth ordering occurs
        triangles.push_back(triangle2);
        triangles.push_back(triangle1);
        renderTriangles(triangles,image);
        image.storeImageToFile(resultFilePath+"Task3.ppm");


        //render a 3d shape (pyramid, etc.)
        triangles.clear();
        image.resetImage();
        CRTTriangle tri1(CRTVector(-4.f,0.5f,-3.f),CRTVector(-1.f,0.5f,-2.f),CRTVector(-1.f,1.5f,-2.5f));
        CRTTriangle tri2(CRTVector(-1.f,0.5f,-2.f),CRTVector(2.f,0.5f,-3.f),CRTVector(-1.f,1.5f,-2.5f));
        CRTTriangle tri3(CRTVector(2.f,0.5f,-3.f),CRTVector(-4.f,0.5f,-3.f),CRTVector(-1.f,1.5f,-2.5f));
        CRTTriangle tri4(CRTVector(-1.f,0.5f,-2.f),CRTVector(-4.f,0.5f,-3.f),CRTVector(2.f,0.5f,-3.f));
        tri1.color = {255,0,0};
        tri2.color = {0,255,0};
        tri3.color = {0,0,255};
        tri4.color = {255,255,255};
        triangles.push_back(tri1);
        triangles.push_back(tri2);
        triangles.push_back(tri3);
        triangles.push_back(tri4);
        renderTriangles(triangles,image);
        image.storeImageToFile(resultFilePath+"Task4.ppm");
    }
    void cleanup() override {
        triangles.clear();
        image.clear();
        //destroy
    }
};