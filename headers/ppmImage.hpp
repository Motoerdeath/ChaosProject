
#ifndef PPMIMAGE
#define PPMIMAGE
#include <vector>
#include <string>
#include "../headers/globals.h"

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
        PPMImage(int width, int height)
        {
            imageHeight = height;
            imageWidth = width;
            image.resize(height,std::vector<std::vector<int>>(width,std::vector<int>(3)));
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
        static std::vector<int> generateRandomColorVariance(std::vector<int> base_color);

    private:
        int imageWidth;
        int imageHeight;
        int maxColorValue;
        std::vector<std::vector<std::vector<int>>> image;
        std::vector<std::vector<GridCell>> grid;
        std::vector<Shape> shapes;
};

#endif