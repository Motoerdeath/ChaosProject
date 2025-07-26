#include "../headers/homework.hpp"
#include "../headers/ppmImage.hpp"
#include <time.h>
#include <random>
#include <string>
class Homework2 : public Homework {
    public:
        Homework2() {setup();};
        void execute() override {
            run();
            cleanup();
        }
    private:
    std::string resultFilePath1 = "../outputs/02_Fundamentals/Task1.ppm";
    std::string resultFilePath2 = "../outputs/02_Fundamentals/Task2.ppm";
    PPMImage image;

    void setup() override {
         //basic config
        const int width = 1920;
        const int height = 1080;
        const int gridX = 4;
        const int gridY = 4;
        //init randomness
        srand(time(NULL));
        //create Image
        
        image = PPMImage(width,height);

        image.configureGrid(gridX, gridY);
        

    }
    void run() override {
        image.renderRandomizedBackground();
        image.storeImageToFile(resultFilePath1);
        //image.clear();

        int radius = 200;
        CRTVector circleColor(1.f,0.f,0.f);
        CRTVector backgroundColor(0.f,0.5f,0.f);
        for(int y =0; y < 1080;y++) {
            for(int x = 0; x < 1920; x++) {
                if((x-960)*(x-960) + (y-540)* (y-540) <= radius*radius) {
                    image.setPixel(circleColor,x,y);
                } else {
                    image.setPixel(backgroundColor,x,y);
                }
            }
        }
        image.storeImageToFile(resultFilePath2);
    }   
    void cleanup() override {
        image.clear();
    }
};