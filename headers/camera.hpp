#ifndef CRTCAMERA
#define CRTCAMERA

#include "crtVector.hpp"
#include "matrix.hpp"
enum Axis{
    X,
    Y,
    Z
};

class CRTCamera {

    public:
        CRTCamera();
        CRTCamera(CRTVector pos, CRTVector dir);
        void pan(float ang, Axis axis);
        void pan(float ang, CRTVector rotationAxis);
        void truck(CRTVector movVector);
    private:
    CRTVector cameraPosition;
    CRTMatrix rotationMatrix;
    CRTVector cameraDirection;
};



#endif