#include "../headers/renderer.hpp"
#include <cfloat>
#include <iostream>
#include <stdexcept>


CRTRenderer::CRTRenderer(CRTScene* scene) : scene(scene){
    scene2 = std::unique_ptr<CRTScene>(scene);
    CRTSettings* settings = scene->getSettings();
    image = PPMImage(settings->imageWidth,settings->imageHeight,255.f);
};
void CRTRenderer::render() {
    
    //iterate over all pixels
    CRTSettings* settings = scene->getSettings();
    for(int y = 0; y < settings->imageHeight;y++) {
        for(int x = 0; x < settings->imageWidth;x++) { 
            CRTRay cameraRay = scene->sceneCamera.generateCameraRay(y, x);
            CRTVector color = traceCameraRay(cameraRay);


            image.setPixel(color,x,y);
        }
    }
    //init FinalColor
}

bool CRTRenderer::intersect(const CRTRay& ray,Intersection& isect) {
    bool foundIntersection = false;
    float closestIntersectionDistance = FLT_MAX;
    int materialID;
    int objectID;
    CRTVector geoNormal;
    CRTVector shadingNormal;
    CRTVector baryCoords;
    CRTVector position;
    CRTTriangle isectTriangle;

    for(int i = 0; i < scene->sceneObjects.size(); i++) {
        CRTMesh* object = &(scene->sceneObjects[i]);
        for(int k = 0; k < object->triangleVertIndices.size();k+=3) {
            CRTTriangle triangle(object->triangleVertices[object->triangleVertIndices[k]],
                                object->triangleVertices[object->triangleVertIndices[k+1]],
                                object->triangleVertices[object->triangleVertIndices[k+2]]);
            float t;
            //shoot shadowRay
            if(ray.type == ShadowRay){
                if(CRTRay::intersectTriangle(ray,triangle,t, true) && t < closestIntersectionDistance) {
                    return true;
                }
            }
            if(ray.type == CameraRay) {
                if(CRTRay::intersectTriangle(ray,triangle,t, false) && t < closestIntersectionDistance) {
                    
                }
            }

        }
    }
    return false;
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
    CRTTriangle isectTriangle;

    for(int i = 0; i < scene->sceneObjects.size(); i++) {
        CRTMesh* object = &(scene->sceneObjects[i]);
        for(int k = 0; k < object->triangleVertIndices.size();k+=3) {
            int triangleFirstIndex = object->triangleVertIndices[k];
            CRTTriangle triangle(object->triangleVertices[object->triangleVertIndices[k]],
                                object->triangleVertices[object->triangleVertIndices[k+1]],
                                object->triangleVertices[object->triangleVertIndices[k+2]]);
            float t;
            if(CRTRay::intersectTriangle(ray,triangle,t, false) && t < closestIntersectionDistance) {
                closestIntersectionDistance = t;
                foundIntersection = true;
                position = ray.rayOrigin + ray.rayDirection*closestIntersectionDistance;
                objectID = i;
                materialID = object->materialID;
                baryCoords = CRTTriangle::calculateBarycentricCoordinates(triangle,position);
                shadingNormal = object->vertexNormals[object->triangleVertIndices[k]]*baryCoords.z +object->vertexNormals[object->triangleVertIndices[k+1]]*baryCoords.x + object->vertexNormals[object->triangleVertIndices[k+2]]*baryCoords.y;
                isectTriangle = triangle;
            }                   
        }     
    }
    if(!foundIntersection) return false;
    isect.intersectionPoint = ray.rayOrigin + ray.rayDirection*closestIntersectionDistance;
    isect.intersectionPoint = isect.intersectionPoint + isectTriangle.normal*0.1f;
    isect.baryCoords = baryCoords;
    isect.mID = materialID;
    isect.shadingNormal = shadingNormal;
    isect.intersectionTriangle = isectTriangle;



    return true;
}

CRTVector CRTRenderer::traceCameraRay(const CRTRay& ray) {
    Intersection isect;
    if(findIntersection(ray, isect)) {

        Material mat = scene->sceneMaterials[isect.mID];
        
        if(mat.type == constant) 
        {
            return mat.albedo;
        } 
        if(mat.type == diffuse) {
            return calculateShading(ray, isect);
        }
        if(mat.type == reflective) {
            return CRTVector(0.f);
        }
        if(mat.type == refractive) {
            return CRTVector(1.f);
        }
        //if we reach this part, something went wrong
        throw std::runtime_error("invalid Material Type");
        
    } else {
        return scene->sceneSettings.backgroundColor;
    }
    //shoot ray through scene determine intersection

    // if no isect then return background

    //if isect then determine material
    //if diffuse then traceShadowRay() and perform shading

    //if reflective then perform traceReflectionRay() and traceRefractionRay()
    //combine results via Fresnel

    //return Color

    //return CRTVector();
}

//determine
bool CRTRenderer::traceShadowRay(const CRTRay& ray) {
            for(CRTMesh object : scene->sceneObjects) {
                for(int k = 0; k < object.triangleVertIndices.size();k+=3) {
                    int triangleFirstIndex = object.triangleVertIndices[k];
                    CRTTriangle triangle(object.triangleVertices[object.triangleVertIndices[k]],
                                        object.triangleVertices[object.triangleVertIndices[k+1]],
                                        object.triangleVertices[object.triangleVertIndices[k+2]]);
                    float t = 1.f;
                    if(CRTRay::intersectTriangle(ray, triangle, t,true)) {
                        return true;
                    }     
                }
            }  
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

CRTVector CRTRenderer::calculateShading(const CRTRay& ray,Intersection& isect) {
    Material mat = scene->sceneMaterials[isect.mID];
        
    if(mat.type == constant) {
        return mat.albedo;
    }
    if(mat.style == flat) {
        return flatShading(ray, isect);
    } else if(mat.style == smooth) {
        return smoothShading(ray, isect);
    } else {
        throw std::runtime_error("invalid Rendering Style");
    }
    
    return CRTVector(0.f);
}
CRTVector CRTRenderer::constantShading(const CRTRay& ray,Intersection& isect) {
    return scene->sceneMaterials[isect.mID].albedo;
}

CRTVector CRTRenderer::flatShading(const CRTRay& ray, Intersection& isect) {

    CRTVector color(0.f);
    CRTVector albedo = scene->sceneMaterials[isect.mID].albedo;
    //remove shadowacne by offsetting position a small amount in the direction of the normal
    CRTVector adjPos = isect.intersectionPoint + isect.intersectionTriangle.normal*0.01f;
    for(Light source : scene->sceneLights) {
        //determine vector to light source from intersectionPoint
        CRTVector lD = (source.lightPosition - adjPos);
        
        if(traceShadowRay(CRTRay(isect.intersectionPoint,lD.normalize()))) continue;
        float lDLength = lD.length();
        //determine if surface is oriented towards light
        float cosLaw = std::max(0.f,CRTVector::dot(lD.normalize(), isect.intersectionTriangle.normal));
        if(cosLaw ==0.f) continue;
        float distanceFallOff = 4*M_PI*lDLength*lDLength;
        CRTVector temp = color +(albedo*(cosLaw*source.lightIntensity/distanceFallOff));
        color = CRTVector(glm::clamp(temp.x,0.f,1.f),glm::clamp(temp.y,0.f,1.f),glm::clamp(temp.z,0.f,1.f));
    }
    return color;
}

CRTVector CRTRenderer::smoothShading(const CRTRay& ray, Intersection& isect) {

    Material  mat = scene->sceneMaterials[isect.mID];
    CRTVector color(0.f);
    //CRTVector albedo = CRTVector(0.4f);
    CRTVector albedo = mat.albedo;
    //remove shadowacne by offsetting position a small amount in the direction of the normal
    //CRTVector adjPos = pos + triangleNormal*0.1f;
    for(Light source : scene->sceneLights) {
        //determine vector to light source from intersectionPoint
        CRTVector lD = (source.lightPosition - isect.intersectionPoint);
        if(traceShadowRay(CRTRay(isect.intersectionPoint,lD.normalize()))) continue;
        float lDLength = lD.length();
        //determine if surface is oriented towards light
        float cosLaw = std::max(0.f,CRTVector::dot(lD.normalize(), isect.shadingNormal));
        if(cosLaw ==0.f) continue;
        float distanceFallOff = 4*M_PI*lDLength*lDLength;
        CRTVector temp = color +(albedo*(cosLaw*source.lightIntensity/distanceFallOff));
        color = CRTVector(glm::clamp(temp.x,0.f,1.f),glm::clamp(temp.y,0.f,1.f),glm::clamp(temp.z,0.f,1.f));
        //color = CRTVector(cosLaw);
        //color = CRTVector(lDLength);
    }
    return color;
}