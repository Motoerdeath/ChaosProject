#include "../headers/ppmImage.hpp"
#include <cstdlib>
#include <vector>
#include <iostream>
#include <fstream>



std::vector<int> PPMImage::generateRandomColorVariance(std::vector<int> baseColor) {
    std::vector<int> res_color = {0,0,0};
    //std::vector<int> ref_color = reference_colors[color];

    float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    res_color[0] = static_cast<int>(static_cast<float>(baseColor[0]) * r);
    res_color[1] = static_cast<int>(static_cast<float>(baseColor[1]) * r);
    res_color[2] = static_cast<int>(static_cast<float>(baseColor[2]) * r);
    return res_color;
}
void PPMImage::clear() {
    image.clear();
    grid.clear();
}
void PPMImage::configureGrid(int x, int y) {
    grid.clear();
    grid.resize(y,std::vector<GridCell>(x));

    for (int i = 0; i < y; i++) {
        for (int j = 0; j < x; j++) {
            GridCell newCell;
            newCell.randomize = true;
            int primaryColor = (rand() % 3);
            glm::vec3 color;
            color[primaryColor] = (rand() % maxColorValue);
            color[(primaryColor+1)%3] = (rand() % (maxColorValue/2));
            color[(primaryColor+2)%3] = (rand() % (maxColorValue/3));
            newCell.colorValue = color;
            grid[i][j]= newCell;
        }
    }
}
void PPMImage::setPixel(int r, int g, int b, int x, int y) {
    image[y][x] = {r,g,b};
}
void PPMImage::renderRandomizedBackground() {

    for (int y = 0; y < imageHeight; y++) {
        for (int x = 0; x < imageWidth; x++) {
            std::vector<int> pixel_color = { 0,0,0 };
            //render background grid
            int sectorX = x /(imageWidth / grid[0].size());
            int sectorY = y / (imageHeight / grid.size());
            std::vector<int> background_color;
            if(grid[sectorY][sectorX].randomize) {
                std::vector<int> color = {(int)grid[sectorY][sectorX].colorValue.r,(int)grid[sectorY][sectorX].colorValue.g,(int)grid[sectorY][sectorX].colorValue.b};
                background_color = PPMImage::generateRandomColorVariance(color);
            } else {
                background_color = {(int)grid[sectorY][sectorX].colorValue.r,(int)grid[sectorY][sectorX].colorValue.g,(int)grid[sectorY][sectorX].colorValue.b};
            }
            pixel_color = background_color;
            //render object

            image[y][x] = pixel_color;
        }
    }
    
}

void PPMImage::storeImageToFile(std::string filePathName) {
    
    std::ofstream ostream;
    ostream.open(filePathName, std::ofstream::out | std::ofstream::trunc | std::ofstream::binary);
    ostream << "P3\n";
    ostream << imageWidth << " " << imageHeight << "\n";
    ostream << maxColorValue << "\n";
    
    for (int y = 0; y < imageHeight; y++) {
        for (int x = 0; x < imageWidth; x++) {
            std::vector<int> pixel_color = image[y][x];
            ostream << " " << pixel_color[0] << " " << pixel_color[1] << " " << pixel_color[2] << " "; 
        }
        ostream << "\n";
    }
    ostream.close();
}


void PPMImage::generateCameraRays() {
        for(int i = 0; i < imageHeight;i++) {
            for(int j = 0; j < imageWidth;j++) {

                float x = static_cast<float>(j) +0.5f;
                float y = static_cast<float>(i) +0.5f;
                
                float ndcX = x/imageWidth;
                float ndcY = y/imageHeight;

                float screenX = ndcX *2.0f -1.0f;
                float screenY = 1.0f - (2.0f*ndcY);
                screenX *= static_cast<float>(imageWidth)/static_cast<float>(imageHeight);

                CRTVector normalizedVector =CRTVector(screenX,screenY,-1.0f).normalize();
                cameraRays[i][j] = CRTRay(CRTVector(0.f,0.f,0.f),normalizedVector);
                
            }
        }
}
//TODO : support shape generation and rendering