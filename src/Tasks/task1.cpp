#include "../headers/task.hpp"
#include "../headers/ppmImage.hpp"
#include <time.h>
#include <random>
#include <string>
class Task1 : public Task {
    public:
        Task1() {setup();};
        void execute() override {
            run();
            cleanup();
        }
    private:
    std::string resultFilePath = "../outputs/02_Fundamentals/output.ppm";
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
        image.storeImageToFile(resultFilePath);
    }
    void cleanup() override {
        image.clear();
    }
};