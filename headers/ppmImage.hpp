
#ifndef PPMIMAGE
#define PPMIMAGE
#include <vector>
#include <string>
#include "../headers/globals.h"
#include "../headers/ray.hpp"

enum ShapeType {
    Circle,
    Triangle,
    Rectangle
};
struct Shape {
    std::vector<std::vector<int>> coordinates;
    std::vector<int> color;
    ShapeType type;
};

class PPMImage{
    public:
        PPMImage() {
            imageHeight = 1080;
            imageWidth = 1920;
            image.resize(imageHeight,std::vector<std::vector<int>>(imageWidth,std::vector<int>(3)));
            cameraRays.resize(imageHeight,std::vector<CRTRay>(imageWidth));
            grid.resize(1,std::vector<GridCell>(1));
            grid[0][0].colorValue = glm::vec3(255);
            grid[0][0].randomize = false;
            maxColorValue = 255;
        }
        PPMImage(int width, int height)
        {
            imageHeight = height;
            imageWidth = width;
            image.resize(height,std::vector<std::vector<int>>(width,std::vector<int>(3)));
            cameraRays.resize(imageHeight,std::vector<CRTRay>(imageWidth));
            grid.resize(1,std::vector<GridCell>(1));
            grid[0][0].colorValue = glm::vec3(255);
            grid[0][0].randomize = false;
            maxColorValue = 255;
        };
        PPMImage(int width, int height, int maxValue)
        {
            imageHeight = height;
            imageWidth = width;
            image.resize(height,std::vector<std::vector<int>>(width,std::vector<int>(3)));
            cameraRays.resize(imageHeight,std::vector<CRTRay>(imageWidth));
            grid.resize(1,std::vector<GridCell>(1));
            grid[0][0].colorValue = glm::vec3(255);
            grid[0][0].randomize = false;
            maxColorValue = maxValue;
        };
        

        void renderRandomizedBackground();
        void renderShapes();
        void createShape();
        void storeImageToFile();
        void storeImageToFile(std::string filePath);
        void configureGrid(int x, int y);
        void reset();
        void clear();
        void setPixel(int r, int g, int b, int x, int y);
        static std::vector<int> generateRandomColorVariance(std::vector<int> base_color);
        void generateCameraRays();
        std::vector<std::vector<CRTRay>> cameraRays;

    private:
        int imageWidth;
        int imageHeight;
        int maxColorValue;
        std::vector<std::vector<std::vector<int>>> image;
        
        std::vector<std::vector<GridCell>> grid;
        std::vector<Shape> shapes;
};

#endif