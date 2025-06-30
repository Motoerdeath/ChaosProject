#include "../headers/homework.hpp"
#include "../headers/ppmImage.hpp"
#include <algorithm>
#include <cstdlib>
#include <time.h>
#include <random>
#include <string>
#include "../headers/ray.hpp"
#include "../headers/camera.hpp"
class Homework3 : public Homework {
    public:
        Homework3() {setup();};
        void execute() override {
            run();
            cleanup();
        }
    private:
    int height = 1080;
    int width = 1920;
    std::string resultFilePath = "../outputs/03_Rays/output.ppm";
    PPMImage image;
    std::vector<std::vector<CRTRay>> pixelRays;
    CRTCamera camera = CRTCamera(CRTVector(0.f),CRTVector(0.f,0.f,-1.f),width,height);

    void setup() override {

        pixelRays.resize(height,std::vector<CRTRay>(width));
        image = PPMImage(width,height);
        //camera = CRTCamera(CRTVector(0.f),CRTVector(0.f,0.f,-1.f),width,height);

    }
    void run() override {
        int height = 1080;
        int width = 1920;
        CRTVector cameraPosition(0.f,0.f,0.f);
        //image.generateCameraRays(cameraPosition);

        for(int i = 0; i < height;i++) {
            for(int j = 0; j < width;j++) {
                CRTRay normalRay = camera.generateCameraRay(i, j);
                //CRTRay normalRay = image.cameraRays[i][j];
                int r = static_cast<int>(glm::clamp(std::abs(normalRay.rayDirection.x) * 255.f,0.f,255.0f));//(int) (rf*255.f);
                int g = static_cast<int>(glm::clamp(std::abs(normalRay.rayDirection.y) * 255.f,0.0f,255.0f));//(int) (gf*255.f);
                int b = static_cast<int>(glm::clamp(std::abs(normalRay.rayDirection.z) * 255.f,0.f,255.0f));//(int) (bf*255.f);
                image.setPixel(r, g, b, j, i);
            }
        }
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

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                CRTRay ray = pixelRays[y][x];
                float rf = (ray.rayDirection.x +1.0f) /2.f;
                float gf = (ray.rayDirection.y +1.0f) /2.f;
                float bf = (ray.rayDirection.z +1.0f) /2.f;
                int r = x % 255;//(int) (rf*255.f);
                int g = y % 255;//(int) (gf*255.f);
                int b = 0;//(int) (bf*255.f);
                image.setPixel(r, g, b, x, y);
            }
        }
            */
        image.storeImageToFile(resultFilePath);
    }
    void cleanup() override {
        pixelRays.clear();
        image.clear();
    }
};