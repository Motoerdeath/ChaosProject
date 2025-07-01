#include "../headers/camera.hpp"

/*
    void pan(float degs);
    void tilt(float degs);
    void roll(float degs);
    void truck(float movDistance);
    void dolly(float movDistance);
    void pedestal(float movDistance);
    void move(CRTVector movVector);
    void rotate(float degs,CRTVector rotationAxis);
*/

void CRTCamera::dolly(float movDistance) {
    cameraPosition + CRTVector(0.f,0.f,movDistance);
}
void CRTCamera::truck(float movDistance) {
    cameraPosition + CRTVector(movDistance,0.f,0.f);
}
void CRTCamera::move(CRTVector& movVector) {
    CRTVector dirAdjustedMoveVec = movVector *rotationMatrix;
    cameraPosition =cameraPosition+dirAdjustedMoveVec;
}
void CRTCamera::pan(float degs) {
    rotationMatrix = rotationMatrix * CRTMatrix::getRotationMatrixAroundY(degs);
}
void CRTCamera::tilt(float degs) {
    rotationMatrix = rotationMatrix * CRTMatrix::getRotationMatrixAroundX(degs);
}
void CRTCamera::roll(float degs) {
    rotationMatrix = rotationMatrix * CRTMatrix::getRotationMatrixAroundZ(degs);
}

void CRTCamera::lookAt(CRTVector target) {
    CRTVector cameraView = CRTVector(0.0f,0.0f,-1.f) * rotationMatrix;
    CRTVector rotationVector = CRTVector::cross(target,cameraView);
    float angle = CRTVector::dot(cameraView.normalize(), target.normalize());
    

}

CRTRay CRTCamera::generateCameraRay(int row, int column) {
    float x = static_cast<float>(column) +0.5f;
    float y = static_cast<float>(row) +0.5f;
                
    float ndcX = x/imageWidth;
    float ndcY = y/imageHeight;

    float screenX = ndcX *2.0f -1.0f;
    float screenY = 1.0f - (2.0f*ndcY);
    screenX *= static_cast<float>(imageWidth)/static_cast<float>(imageHeight);

    CRTVector normalizedVector =(CRTVector(screenX,screenY,-1.0f)*rotationMatrix).normalize();
    return CRTRay(cameraPosition,normalizedVector);
}