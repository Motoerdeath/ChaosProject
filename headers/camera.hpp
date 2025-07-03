#ifndef CRTCAMERA
#define CRTCAMERA

#include "crtVector.hpp"
#include "matrix.hpp"
#include "../headers/ray.hpp"
enum Axis{
    X,
    Y,
    Z
};

class CRTCamera {

    public:
        CRTCamera() : cameraPosition(CRTVector(0.f)),rotationMatrix(CRTMatrix(CRTVector(1.f,0.f,0.f),CRTVector(0.f,1.f,0.f),CRTVector(0.f,0.f,1.f))), imageWidth(1920), imageHeight(1080){}

        CRTCamera(CRTVector pos, CRTMatrix rotMatrix,int width, int height) : 
        cameraPosition(pos),
        rotationMatrix(rotMatrix) {imageWidth = width; imageHeight =height;};
        CRTCamera(CRTVector pos,int width, int height) : 
        cameraPosition(pos),
        rotationMatrix(CRTMatrix(CRTVector(1.f,0.f,0.f),CRTVector(0.f,1.f,0.f),CRTVector(0.f,0.f,1.f))) {imageWidth = width; imageHeight =height;};
        void pan(float degs);
        void tilt(float degs);
        void roll(float degs);
        void truck(float movDistance);
        void dolly(float movDistance);
        void pedestal(float movDistance);
        void move(CRTVector& movVector);
        void rotate(float degs,CRTVector rotationAxis);
        CRTRay generateCameraRay(int row, int column);
        void lookAt(CRTVector target);
    private:
    CRTVector cameraPosition;
    CRTMatrix rotationMatrix;
    int imageWidth;
    int imageHeight;
};



#endif