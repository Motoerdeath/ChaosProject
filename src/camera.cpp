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
    CRTVector dirAdjustedMoveVec = rotationMatrix * movVector;
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
CRTMatrix generalRotationMatrix(const CRTVector& axis,const float degs) {
    const float cosTheta = cos(degs);
    const float sinTheta = sin(degs);
    return CRTMatrix(CRTVector(cosTheta + axis.x*axis.x*(1-cosTheta),axis.x*axis.y*(1-cosTheta)-axis.z*sinTheta, axis.x*axis.z*(1-cosTheta)+axis.y*sinTheta),
                    CRTVector(axis.x*axis.y*(1-cosTheta)+axis.z*sinTheta,cosTheta + axis.y*axis.y*(1-cosTheta),axis.y*axis.z*(1-cosTheta)-axis.x*sinTheta),
                    CRTVector(axis.x*axis.z*(1-cosTheta)-axis.y*sinTheta,axis.y*axis.z*(1-cosTheta)+axis.x*sinTheta,cosTheta + axis.z*axis.z*(1-cosTheta)));
}
void CRTCamera::lookAt(CRTVector target) {
    CRTVector cameraView =  rotationMatrix * CRTVector(0.0f,0.0f,-1.f);
    CRTVector rotationVector = CRTVector::cross(cameraView,(target-cameraPosition));
    float angle = std::acos(CRTVector::dot(cameraView.normalize(), (target-cameraPosition).normalize()));
    rotationMatrix = rotationMatrix * generalRotationMatrix(rotationVector.normalize(), angle);
}

CRTRay CRTCamera::generateCameraRay(int row, int column) {
    float x = static_cast<float>(column) +0.5f;
    float y = static_cast<float>(row) +0.5f;
                
    float ndcX = x/imageWidth;
    float ndcY = y/imageHeight;

    float screenX = ndcX *2.0f -1.0f;
    float screenY = 1.0f - (2.0f*ndcY);
    screenX *= static_cast<float>(imageWidth)/static_cast<float>(imageHeight);

    CRTVector normalizedVector =(rotationMatrix * CRTVector(screenX,screenY,-1.0f)).normalize();
    return CRTRay(cameraPosition,normalizedVector);
}