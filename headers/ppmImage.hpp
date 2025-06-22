#include <vector>
#include "../headers/globals.h"

class PPMImageEditor {
    public:
        
};

class PPMImage{
    public:
        PPMImage(int width, int height)
        {
            imageHeight = height;
            imageWidth = width;
            image.resize(height,std::vector<std::vector<int>>(width,std::vector<int>(3)));
            maxColorValue = 255;
        };
        PPMImage(int width, int height, int maxValue)
        {
            imageHeight = height;
            imageWidth = width;
            image.resize(height,std::vector<std::vector<int>>(width,std::vector<int>(3)));
            maxColorValue = maxValue;
        };
        

        void render();
        void storeImageToFile();
        void configureGrid(int x, int y);
        static std::vector<int> generateRandomColorVariance(std::vector<int> base_color);

    private:
        int imageWidth;
        int imageHeight;
        int maxColorValue;
        std::vector<std::vector<std::vector<int>>> image;
        std::vector<std::vector<GridCell>> grid;


};