#include "glm/ext/vector_float3.hpp"
#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>
#include <time.h>
#include <utility>
#include <tuple>
#include <random>
#include "../headers/ppmImage.hpp"

std::vector<std::vector<int>> reference_colors{ {255,0,0},{0,255,0},{0,0,255},{255,128,0},{255,255,0},{127,0,255},{255,0,127} };
enum BaseColor {
    Red = 0,
    Green = 1,
    Blue = 2,
    Orange,
    Yellow,
    Purple,
    Pink,

    NumColors,
};
enum ShapeType {
    Circle,
    Triangle,
    Rectangle
};
struct Shape {
    std::vector<std::vector<int>> coordinates;
    BaseColor color;
    ShapeType type;
};

std::vector<int> generateRandomColorVariance(BaseColor color) {
    std::vector<int> res_color = {0,0,0};
    std::vector<int> ref_color = reference_colors[color];

    float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    res_color[0] = static_cast<int>(static_cast<float>(ref_color[0]) * r);
    res_color[1] = static_cast<int>(static_cast<float>(ref_color[1]) * r);
    res_color[2] = static_cast<int>(static_cast<float>(ref_color[2]) * r);
    return res_color;
}
std::vector<int> generateRandomColorVariance(std::vector<int> base_color) {
    std::vector<int> res_color = {0,0,0};
    //std::vector<int> ref_color = reference_colors[color];

    float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    res_color[0] = static_cast<int>(static_cast<float>(base_color[0]) * r);
    res_color[1] = static_cast<int>(static_cast<float>(base_color[1]) * r);
    res_color[2] = static_cast<int>(static_cast<float>(base_color[2]) * r);
    return res_color;
}

int main() {

    //basic config
    const int width = 1920;
    const int height = 1080;
    const int maxColorValue = 255;
    const int gridX = 2;
    const int gridY = 4;
    //init randomness
    srand(time(NULL));
    //create Image
    
    PPMImage image(width,height);

    image.configureGrid(gridX, gridY);
    
    image.render();
    image.storeImageToFile();
    
/*
    //render to image, using selected grid configuration and shapes
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            std::vector<int> pixel_color = { 0,0,0 };

            //render background grid
            int sectorX = x /(width / gridX);
            int sectorY = y / (height / gridY);
            std::vector<int> background_color;
            if(grid[sectorY][sectorX].randomize) {
                std::vector<int> color = {(int)grid[sectorY][sectorX].colorValue.r,(int)grid[sectorY][sectorX].colorValue.g,(int)grid[sectorY][sectorX].colorValue.b};
                background_color = generateRandomColorVariance(color);
            } else {
                background_color = {(int)grid[sectorY][sectorX].colorValue.r,(int)grid[sectorY][sectorX].colorValue.g,(int)grid[sectorY][sectorX].colorValue.b};
            }
            pixel_color = background_color;
            //render object
            int xDistance = (x-circleCoordinates[0]) * (x-circleCoordinates[0]);
            int yDistance = (y-circleCoordinates[1]) * (y-circleCoordinates[1]);
            if(xDistance + yDistance < circleRadius*circleRadius) {
                pixel_color = {(int)circleColor.r,(int)circleColor.g,(int)circleColor.b};
            }
            image[y][x] = pixel_color;
        }
    }
    */
    return 0;
}