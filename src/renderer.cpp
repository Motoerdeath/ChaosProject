#include "../headers/renderer.hpp"
#include "glm/geometric.hpp"
#include <cfloat>
#include <cmath>
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
            CRTVector color = traceRay(cameraRay);


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

bool CRTRenderer::findIntersection(const CRTRay& ray,Intersection& isect, const float maxT) {

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
            if(CRTRay::intersectTriangle(ray,triangle,t, true) && t < closestIntersectionDistance) {
                closestIntersectionDistance = t;
                if(t > maxT) continue;
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
    isect.intersectionPoint = isect.intersectionPoint + isectTriangle.normal*0.001f;

    isect.baryCoords = baryCoords;
    isect.mID = materialID;
    isect.shadingNormal = shadingNormal;
    isect.intersectionTriangle = isectTriangle;
    



    return true;
}


CRTVector CRTRenderer::traceRay(const CRTRay& ray, const float maxT) {
    //if(ray.rayDepth >= maxDepth) return scene->getBackgroundColor();
    if(ray.rayDepth >= maxDepth) return scene->getBackgroundColor();

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
            return calculateShading(ray, isect);
        }
        if(mat.type == refractive) {
            return calculateShading(ray, isect);
        }
        //if we reach this part, something went wrong
        assert(false);
        
    } else {
        return scene->sceneSettings.backgroundColor;
    }

}


//determine
bool CRTRenderer::traceShadowRay(const CRTRay& ray,const float maxT) {
            for(CRTMesh object : scene->sceneObjects) {
                for(int k = 0; k < object.triangleVertIndices.size();k+=3) {
                    int triangleFirstIndex = object.triangleVertIndices[k];
                    CRTTriangle triangle(object.triangleVertices[object.triangleVertIndices[k]],
                                        object.triangleVertices[object.triangleVertIndices[k+1]],
                                        object.triangleVertices[object.triangleVertIndices[k+2]]);
                    float t = 1.f;
                    if(CRTRay::intersectTriangle(ray, triangle, t,true)) {
                        if(t > maxT) continue;
                        return true;
                    }     
                }
            }  
    return false;
}



CRTVector CRTRenderer::calculateShading(const CRTRay& ray,Intersection& isect) {
    Material mat = scene->sceneMaterials[isect.mID];
    
    if(ray.rayDepth>= maxDepth) {
        return scene->getBackgroundColor();  
    } else if(mat.type == constant) {
        return constantShading(ray, isect);
    } else if(mat.type == diffuse) {
        return diffuseShading(ray, isect);
    } else if(mat.type == reflective) {
        return reflectiveShading(ray, isect);
    } else if(mat.type == refractive) {
        return refractiveShading(ray, isect);
    } else {
        throw std::runtime_error("invalid Rendering Style");
    }
    
    return CRTVector(0.f);
}
CRTVector CRTRenderer::constantShading(const CRTRay& ray,Intersection& isect) {
    return scene->sceneMaterials[isect.mID].albedo;
}

CRTVector CRTRenderer::refractiveShading(const CRTRay& ray,Intersection& isect) {
    
    Material mat = scene->sceneMaterials[isect.mID];
    CRTVector normal = isect.geomNormal;
    CRTVector I = ray.rayDirection.normalize();
    float entryIOR = 1.f;
    float exitIOR = mat.ior;
    
    if(CRTVector::dot(I, normal)>0.f){
        normal = normal* -1.f;
        entryIOR = mat.ior;
        exitIOR = 1.f;
    }

    float relativeIOR = entryIOR/exitIOR;

    float cosAlpha = -1.f*CRTVector::dot(I, normal);
    //Total Internal Reflection check
    if(sqrt(1-cosAlpha*cosAlpha)> entryIOR/exitIOR) {
        return CRTVector(1.f);
    }
    float sinBeta = sqrt(1-cosAlpha*cosAlpha) *entryIOR / exitIOR;
    float cosBeta = sqrt(1-sinBeta*sinBeta);
    CRTVector C = (I +  cosAlpha*normal).normalize();
    CRTVector B = C*sinBeta;
    CRTVector A = cosBeta * -1.f * normal;
    CRTVector R = A +B;
    CRTRay refractionRay(isect.intersectionPoint + -1.f*0.001f*normal,R);

    glm::vec3 vectorI =glm::vec3(ray.rayDirection.x,ray.rayDirection.y,ray.rayDirection.z);
    glm::vec3 vectorN =glm::vec3(normal.x,normal.y,normal.z);
    glm::vec3 vectorR = glm::refract(vectorI,vectorN,relativeIOR);
    glm::vec3 vectorf = glm::reflect(vectorI,vectorN);
    CRTVector vR(vectorR.x,vectorR.y,vectorR.z);
    CRTVector vf(vectorf.x,vectorf.y,vectorf.z);
    //glm::refract(ray.rayDirection)
    refractionRay.rayDepth = ray.rayDepth+1;
    refractionRay.type = RefractionRay;
    return traceRay(refractionRay);
}
/*
CRTVector CRTRenderer::refractiveShading(const CRTRay& ray,Intersection& isect) {
    Material mat = scene->sceneMaterials[isect.mID];
    ;
    CRTVector normal = isect.geomNormal;

    /*
    if(mat.style == smooth) {
        normal = isect.shadingNormal;
    } else {
        normal = isect.geomNormal;
    }
        

    float fresnel = CRTRenderer::fresnel(ray,normal);
    float entryIOR = 1.f;
    float exitIOR = mat.ior;
    if(CRTVector::dot(ray.rayDirection.normalize(), normal)>0.f){
        normal = normal* -1.f;
        entryIOR = mat.ior;
        exitIOR = 1.f;
    }
    float cosAlpha = -1.f * CRTVector::dot(ray.rayDirection.normalize(), normal);
    float sinAlpha = sqrt(1.f-(cosAlpha*cosAlpha));
    float relativeIOR = entryIOR/exitIOR;



    CRTRay reflectionRay(isect.intersectionPoint,CRTRay::reflect(ray.rayDirection.normalize(), normal));
    reflectionRay.type = ReflectionRay;
    reflectionRay.rayDepth = ray.rayDepth+1;
    //if(1.f-(cosAlpha*cosAlpha) > 1.0f){return CRTVector(0.f);}

    float sinT2 = relativeIOR*relativeIOR * (1.f-cosAlpha*cosAlpha);
    if(sinAlpha >= relativeIOR) return traceRay(reflectionRay);
    //if(sinAlpha >= relativeIOR) return CRTVector(0.f);
    float cosT = sqrt(1.f-sinT2);
    CRTVector t = relativeIOR*ray.rayDirection + (relativeIOR*cosAlpha - cosT) * normal;
    glm::vec3 vectorI =glm::vec3(ray.rayDirection.x,ray.rayDirection.y,ray.rayDirection.z);
    glm::vec3 vectorN =glm::vec3(normal.x,normal.y,normal.z);
    glm::vec3 vectorR = glm::refract(vectorI,vectorN,relativeIOR);
    glm::vec3 vectorf = glm::reflect(vectorI,vectorN);
    CRTVector vR(vectorR.x,vectorR.y,vectorR.z);
    CRTVector vf(vectorf.x,vectorf.y,vectorf.z);
    float sinBeta = sinAlpha*relativeIOR;
    float cosBeta = sqrt(1.f-(sinBeta*sinBeta));
    CRTVector A = cosBeta*(normal*-1.f);
    CRTVector C = (ray.rayDirection.normalize() + cosAlpha*normal).normalize();
    CRTVector B = C *sinBeta;
    CRTVector R = A+B;
    CRTVector refractionOrigin = isect.intersectionPoint + (normal*(-1.f)*0.01f);
    //std::cout << "Begin" << std::endl;
    //std::cout << fresnel << std::endl;
    //std::cout << reflectionRay.rayDirection.x<< ";"<<reflectionRay.rayDirection.y <<";" <<reflectionRay.rayDirection.z  <<std::endl;
    //std::cout << vf.x<< ";"<<vf.y <<";" <<vf.z  <<std::endl;
    //std::cout << t.x<< ";"<<t.y <<";" <<t.z  <<std::endl;


    //schlick approximation for Fresnel term/specular reflectance
    float r0 = (entryIOR-exitIOR)/(entryIOR+exitIOR);
    r0 = r0*r0;
    float F = r0+((1.f-r0)*pow((1.f - cosAlpha),5));
    //std::cout << F << std::endl;
    //CRTVector t = -1.f*(sinBeta/sinAlpha)*(ray.rayDirection.normalize() - normal*cosAlpha) - normal*cosBeta;
    CRTRay refractionRay(refractionOrigin,vR);
    refractionRay.type = RefractionRay;
    refractionRay.rayDepth = ray.rayDepth+1;
    //return traceRay(reflectionRay)*F + (1.f-F) *traceRay(refractionRay);
    return traceRay(refractionRay);
}
*/


CRTVector CRTRenderer::reflectiveShading(const CRTRay& ray,Intersection& isect) {
    Material mat = scene->sceneMaterials[isect.mID];
    CRTVector normal;
    if(mat.style == smooth) {
        normal = isect.shadingNormal;
    } else {
        normal = isect.geomNormal;
    }
    CRTRay reflectionRay(isect.intersectionPoint,CRTRay::reflect(ray.rayDirection, normal));
    reflectionRay.type = ReflectionRay;
    reflectionRay.rayDepth = ray.rayDepth+1;
            
    CRTVector shadingResult = traceRay(reflectionRay);
    return CRTVector(mat.albedo.x*shadingResult.x,mat.albedo.y*shadingResult.y,mat.albedo.z*shadingResult.z);
}

CRTVector CRTRenderer::diffuseShading(const CRTRay& ray,Intersection& isect ) {
    Material mat = scene->sceneMaterials[isect.mID];
    if(mat.style ==flat) {
        return flatShading(ray, isect);
    } else {
        return smoothShading(ray, isect);
    }
}

CRTVector CRTRenderer::flatShading(const CRTRay& ray, Intersection& isect) {

    CRTVector color(0.f);
    CRTVector albedo = scene->sceneMaterials[isect.mID].albedo;
    //remove shadowacne by offsetting position a small amount in the direction of the normal
    //CRTVector adjPos = isect.intersectionPoint + isect.intersectionTriangle.normal*0.01f;
    for(Light source : scene->sceneLights) {
        //determine vector to light source from intersectionPoint
        CRTVector lD = (source.lightPosition - isect.intersectionPoint);
        CRTRay shadowRay(isect.intersectionPoint,lD.normalize());
        shadowRay.type= ShadowRay;
        Intersection shadowIsect;
        if(findIntersection(shadowRay, shadowIsect,lD.length())) {
            Material mat = scene->sceneMaterials[shadowIsect.mID];
            if(mat.type != refractive) continue;
        }
        //if(traceShadowRay(shadowRay,lD.length())) continue;
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
        float lDLength = lD.length();
        CRTRay shadowRay(isect.intersectionPoint,lD.normalize());
        shadowRay.type= ShadowRay;
        if(traceShadowRay(shadowRay,lDLength)) continue;
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
//this represents a simplified version of the Fresnel Equation
float CRTRenderer::fresnel(const CRTRay& ray,const CRTVector& normal) {
    float dotIN=CRTVector::dot(ray.rayDirection.normalize(), normal);
    return 0.5f*(std::pow(1.f+dotIN,5));
}

float fresnel_schlick(const CRTRay& ray,const CRTVector& normal,const float etai,const float eta2) {
    float r0 = (etai-eta2)/(etai+eta2);
    r0 = r0*r0;
    return r0+((1.f-r0)*pow((1.f - CRTVector::dot(ray.rayDirection.normalize(), normal)),5));
}