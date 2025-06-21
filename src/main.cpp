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
struct GridCell {
    glm::vec3 colorValue;
    bool randomize;
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
    const int gridX = 4;
    const int gridY = 4;

    //create Image
    std::vector<std::vector<std::vector<int>>> image;
    image.resize(height,std::vector<std::vector<int>>(width,std::vector<int>(3)));

    //initialize rand()
    srand(time(NULL));


    //generate shapes
    std::vector<Shape> objects;
    Shape someCircle;
    someCircle.coordinates = { std::vector<int>(width/2,height/2), std::vector<int>(50,0)};
    someCircle.color = Red;
    objects.push_back(someCircle);
    std::vector<int> circleCoordinates = {width/2,height/2};
    int circleRadius = 100;
    int primaryColor2 = (rand() % 3);
    glm::vec3 circleColor;
    circleColor[primaryColor2] = (rand() % maxColorValue);
    circleColor[(primaryColor2+1)%3] = (rand() % (maxColorValue/2));
    circleColor[(primaryColor2+2)%3] = (rand() % (maxColorValue/3));


    //generate Grid configuration
    GridCell grid[gridY][gridX];
    for (int i = 0; i < gridY; i++) {
        for (int j = 0; j < gridX; j++) {
            GridCell newCell;
            newCell.randomize = true;
            int primaryColor = (rand() % 3);
            glm::vec3 color;
            color[primaryColor] = (rand() % maxColorValue);
            color[(primaryColor+1)%3] = (rand() % (maxColorValue/2));
            color[(primaryColor+2)%3] = (rand() % (maxColorValue/3));
            newCell.colorValue = color;
            grid[i][j]= newCell;
            //gridColorAssignments[i][j] = static_cast<BaseColor> (rand() % BaseColor::NumColors);
        }
    }

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
    

    //store image to ppm file
    std::cout << "start"<< std::endl;
    std::ofstream ostream;
    ostream.open("../outputs/output.ppm", std::ofstream::out | std::ofstream::trunc | std::ofstream::binary);
    ostream << "P3\n";
    ostream << width << " " << height << "\n";
    ostream << maxColorValue << "\n";
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            std::vector<int> pixel_color = image[y][x];
            ostream << " " << pixel_color[0] << " " << pixel_color[1] << " " << pixel_color[2] << " "; 
        }
        ostream << "\n";
    }

    return 0;
}