#include "../headers/camera.hpp"
#include <cmath>
#include "../headers/globalSettings.hpp"

//instead of dolly/truck/pedestal i allow for a single move function that gets an arbitrary vector
//  and moves the cameraposition by that vector in its own coordinates system
void CRTCamera::move(CRTVector& movVector) {
    CRTVector dirAdjustedMoveVec = movVector * rotationMatrix;
    cameraPosition =cameraPosition+dirAdjustedMoveVec;
}
void CRTCamera::pan(float degs) {
    rotationMatrix = rotationMatrix * CRTMatrix::getRotationMatrixAroundY(degs);
}
void CRTCamera::tilt(float degs) {
    rotationMatrix =  rotationMatrix * CRTMatrix::getRotationMatrixAroundX(degs);
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

//rotates the camera to look at a certain target position
void CRTCamera::lookAt(CRTVector target) {
    CRTVector cameraView = CRTVector(0.0f,0.0f,-1.f) * rotationMatrix;
    CRTVector rotationVector = CRTVector::cross((target-cameraPosition),cameraView);
    float angle = std::acos(CRTVector::dot(cameraView.normalize(), (target-cameraPosition).normalize()));
    rotationMatrix = rotationMatrix * generalRotationMatrix(rotationVector.normalize(), angle);
}


//creates a normalized ray direction from the camera origin to a pixel on the image plane
CRTRay CRTCamera::generateCameraRay(int row, int column, bool jitter) {
    float offsetX = 0.f;
    float offsetY = 0.f;
    if(jitter) {
        offsetX = dist(mt);
        offsetY = dist(mt);
    }
    float x = static_cast<float>(column) +0.5f + offsetX;
    float y = static_cast<float>(row) +0.5f + offsetY;
                
    float ndcX = x/imageWidth;
    float ndcY = y/imageHeight;

    float screenX = ndcX *2.0f -1.0f;
    float screenY = 1.0f - (2.0f*ndcY);
    screenX *= static_cast<float>(imageWidth)/static_cast<float>(imageHeight);

    float FOVrads = fov*M_PI/180;
    screenX *= std::tan(FOVrads/2.f);
    screenY *= std::tan(FOVrads/2.f);
    CRTVector normalizedVector =(CRTVector(screenX,screenY,-1.0f) * rotationMatrix).normalize();
    return CRTRay(cameraPosition,normalizedVector);
}

void CRTCamera::setupRNG() {

    std::random_device rd;
    mt = std::mt19937(rd());
    dist = std::uniform_real_distribution<float>(-0.5f,0.5f);

}