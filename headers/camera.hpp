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
        CRTCamera();

        CRTCamera(CRTVector pos, CRTVector dir,int width, int height) : 
        cameraPosition(pos),
        cameraDirection(dir),
        rotationMatrix(CRTVector(1.f,0.f,0.f),CRTVector(0.f,1.f,0.f), CRTVector(0.f,0.f,1.f)
    ) {imageWidth = width; imageHeight =height;};
        
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
    CRTVector cameraDirection;
    int imageWidth;
    int imageHeight;
};



#endif