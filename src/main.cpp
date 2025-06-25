#include "glm/ext/vector_float3.hpp"
#include <cmath>
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
#include "glm/geometric.hpp"
#include "../headers/crtVector.hpp"
//#include "../headers/task.hpp"
#include "../src/Tasks/task1.cpp"
#include "../src/Tasks/task2.cpp"

bool FUNDAMENTALS = true;
bool RAYS = true;

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


int main() {
    srand(time(NULL));
    const int width = 1920;
    const int height = 1080;
    const int maxColorValue = 255;
    const int gridX = 4;
    const int gridY = 4;
    Task1 task1;
    task1.execute();

    /*
    //basic config

    //init randomness
    
    //create Image
    
    PPMImage image(width,height);

    image.configureGrid(gridX, gridY);
    
    image.renderRandomizedBackground();
    image.storeImageToFile();
*/

    //init pinhole to screenspace Rays
    /*
    std::vector<std::vector<CRTRay>> image2;
    image2.resize(height,std::vector<CRTRay>(width));

    //generate normalized rays for each screen pixel
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
            image2[i][j] = newRay;
        }
    }
    //render normalizedRays

    std::cout << "Begin writing normalized Ray Image to File"<< std::endl;
    std::ofstream ostream;
    ostream.open("../outputs/03_Rays/output.ppm", std::ofstream::out | std::ofstream::trunc | std::ofstream::binary);
    ostream << "P3\n";
    ostream << width << " " << height << "\n";
    ostream << maxColorValue << "\n";
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            CRTRay ray = image2[y][x];
            float rf = (ray.rayDirection.x +1.0f) /2.f;
            float gf = (ray.rayDirection.y +1.0f) /2.f;
            float bf = (ray.rayDirection.z +1.0f) /2.f;
            int r = (int) (rf*255.f);
            int g = (int) (gf*255.f);
            int b = (int) (bf*255.f);
            ostream << " " << r << " " << g << " " << b << " "; 
        }
        ostream << "\n";
    }
    ostream.close();
    std::cout << "wrote Image to File"<< std::endl;
    std::cout <<"did it" << std::endl;
    
*/

Task2 task2;
task2.execute();




//Task3
    CRTVector a(0.56f,1.11f,1.23f);
    CRTVector b(0.44f,-2.368f,-0.54f);
    CRTVector c(-1.56f,0.15f,-1.92f);

    CRTVector ab= b-a;
    CRTVector ac= c-a;
    CRTVector n = ab.cross(ac);
    n = n.normalize();
    std::cout << n.x << n.y << n.z <<std::endl;;
    return 0;
}