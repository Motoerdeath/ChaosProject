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

    //basic config
    const int width = 1920;
    const int height = 1080;
    const int maxColorValue = 255;
    const int gridX = 4;
    const int gridY = 4;
    //init randomness
    srand(time(NULL));
    //create Image
    
    PPMImage image(width,height);

    image.configureGrid(gridX, gridY);
    
    image.renderRandomizedBackground();
    image.storeImageToFile();


    //init pinhole to screenspace Rays
    std::vector<std::vector<CRTRay>> image2;
    image2.resize(height,std::vector<CRTRay>(width));

    //generate normalized rays for each screen pixel
    for(int i = 0; i < height;i++) {
        for(int j = 0; j < width;j++) {

            float x = (float) j +0.5f;
            float y = (float) (height-i) -0.5f;
            //float y = (float) i +0.5f;
            x /= width;
            y /= height;
            x = (2.f*x)-1.0f;
            y = 1.f - (2.f*y);
            x *= ((float) width / (float) height);
            //y *= ((float) width / (float) height);
            CRTVector2 rayDir2(x,y,-1.f);
            CRTVector rayDirection;
            rayDirection.x = x;
            rayDirection.y = y;
            rayDirection.z = -1.f;
            glm::vec3  rayDir = glm::vec3(x,y,-1.f);
            /*
            glm::vec3 norm = glm::normalize(rayDir);
            rayDirection.x = norm.x;
            rayDirection.y = norm.y;
            rayDirection.z = norm.z;
            */
            float rayLength = std::sqrt(rayDirection.x*rayDirection.x + rayDirection.y *rayDirection.y + rayDirection.z * rayDirection.z);
            CRTVector normalizedRayDirection;
            normalizedRayDirection.x = rayDirection.x / rayLength;
            normalizedRayDirection.y = rayDirection.y / rayLength;
            normalizedRayDirection.z = rayDirection.z / rayLength;
            CRTVector2 normalizedRayDir = rayDir2.normalize();
            CRTVector origin;
            CRTVector2 o(0.f);
            CRTRay newRay(o, normalizedRayDir);
            origin.x =0.f;
            origin.y= 0.f;
            origin.z = 0.f;
            newRay.rayOrigin = o;
            newRay.rayDirection = normalizedRayDir;
            image2[i][j] = newRay;
            /*
            CRTRay newRay;
            CRTVector vector;
            vector.x =0.f;
            vector.y= 0.f;
            vector.z = 0.f;
            newRay.rayOrigin = vector;
            CRTVector pixelpos;
            pixelpos.x = (float)j +0.5f;
            pixelpos.y = (float)(height-i) -0.5f;
            pixelpos.x = -1.f;
            newRay.rayDirection = pixelpos;
            

            //normalize;
            float rayLength = std::sqrt(pixelpos.x*pixelpos.x + pixelpos.y +pixelpos.y + pixelpos.z + pixelpos.z);
            CRTVector normalizedRayDirection;
            normalizedRayDirection.x = pixelpos.x / rayLength;
            normalizedRayDirection.y = pixelpos.y / rayLength;
            normalizedRayDirection.z = pixelpos.z / rayLength;
            */

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
    

    return 0;
}