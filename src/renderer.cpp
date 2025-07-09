#include "../headers/renderer.hpp"
#include <cfloat>
#include <iostream>


void CRTRenderer::render() {
    
    //iterate over all pixels
    CRTSettings* settings = scene->getSettings();
    for(int x = 0; x < settings->imageHeight;x++) {
        for(int y = 0; y < settings->imageWidth;y++) { 
            CRTRay cameraRay = scene->sceneCamera.generateCameraRay(y, x);
            CRTVector color = traceCameraRay(cameraRay);
        }
    }
    //init FinalColor


}

bool CRTRenderer::findIntersection(const CRTRay& ray,Intersection& isect) {

    bool foundIntersection = false;
    float closestIntersectionDistance = FLT_MAX;
    int materialID;
    int objectID;
    CRTVector geoNormal;
    CRTVector shadingNormal;
    CRTVector baryCoords;
    CRTVector position;

    for(int i = 0; i < scene->sceneObjects.size(); i++) {
        CRTMesh* object = &scene->sceneObjects[i];
        for(int k = 0; k < object->triangleVertIndices.size();k+=3) {
            int triangleFirstIndex = object->triangleVertIndices[k];
            CRTTriangle triangle(object->triangleVertices[object->triangleVertIndices[k]],
                                object->triangleVertices[object->triangleVertIndices[k+1]],
                                object->triangleVertices[object->triangleVertIndices[k+2]]);
            float t;
            if(CRTRay::intersectTriangle(ray,triangle,t, false) && t < closestIntersectionDistance) {
                closestIntersectionDistance = t;
                foundIntersection = true;

                objectID = i;
                materialID = object->materialID;
                baryCoords = CRTTriangle::calculateBarycentricCoordinates(triangle,position);
            }                   
        }     
    }
    if(!foundIntersection) return false;
    isect.intersectionPoint = ray.rayOrigin + ray.rayDirection*closestIntersectionDistance;
    isect.baryCoords = baryCoords;
    isect.mID = materialID;



    return true;
}

CRTVector CRTRenderer::traceCameraRay(const CRTRay& ray) {

    //shoot ray through scene determine intersection

    // if no isect then return background

    //if isect then determine material
    //if diffuse then traceShadowRay() and perform shading

    //if reflective then perform traceReflectionRay() and traceRefractionRay()
    //combine results via Fresnel

    //return Color

    return CRTVector();
}

//determine
bool CRTRenderer::traceShadowRay(const CRTRay& ray) {
    //add shadow bias
    //trace shadowRay through scene determine if intersection exists
    //if no return true;
    //if yes return false
    return false;
}

CRTVector CRTRenderer::traceReflectionRay(const CRTRay& ray) {
    //end Raytracing early if maxdepth has been reached
    //return background color instead
    //TODO: add environment mapping for more interesting scenes
    if(ray.rayDepth >= maxDepth) return scene->getBackgroundColor();
    return CRTVector();
}

CRTVector CRTRenderer::traceRefractionRay(const CRTRay& ray) {
    //end Raytracing early if maxdepth has been reached
    //return background color instead
    //TODO: add environment mapping for more interesting scenes
    if(ray.rayDepth >= maxDepth) return scene->getBackgroundColor();

    return CRTVector();
}