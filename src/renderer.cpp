#include "../headers/renderer.hpp"
#include "../include/glm/geometric.hpp"
#include <algorithm>
#include <cfloat>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <chrono>


CRTRenderer::CRTRenderer(CRTScene* scene) : scene(scene){
    scene2 = std::unique_ptr<CRTScene>(scene);
    CRTSettings* settings = scene->getSettings();
    image = PPMImage(settings->imageWidth,settings->imageHeight,255.f);
};
void CRTRenderer::setupTriangleAccessStructure() {

    as.createTriangleSoup(scene->sceneObjects);
    as.buildAS();
    std::printf("hey");
}
void CRTRenderer::render() {
    /*
    aTexture.albedo = CRTVector(0.f,0.f,1.f);
    aTexture.type = albedoTexture;
    aTexture.innerColor = CRTVector(1.f,0.f,0.f);
    aTexture.edgeColor = CRTVector(0.f,1.f,0.f);
    aTexture.edgeWidth = 0.1f;
    aTexture.type=edgeTexture;
    */
    generateBoundingBox();


    //as.createTriangleSoup(scene->sceneObjects);
    //as.buildAS();
    //iterate over all pixels
    CRTSettings* settings = scene->getSettings();
    for(int y = 0; y < settings->imageHeight;y++) {
        for(int x = 0; x < settings->imageWidth;x++) { 

            auto start = std::chrono::steady_clock::now();//timing
            CRTRay cameraRay = scene->sceneCamera.generateCameraRay(y, x);
            cameraRay.rayDepth = 0;
            cameraRay.type = CameraRay;
            CRTVector color = traceRay(cameraRay);

            //Heatmap and timing stuff
            //auto finish = std::chrono::steady_clock::now();
            //const std::chrono::duration<double> elapsed_seconds{finish - start};
            //float time = elapsed_seconds.count()*100000.f;
            //CRTVector timeColor = temperature(time);
            image.setPixel(color,x,y);
            
        }
        std::cout << "row:" << y+1 <<"/" <<settings->imageHeight << " finished" << std::endl;
    }
}
void CRTRenderer::renderRegion(const int startX,const int startY,const int regionWidth, const int regionHeight) {

    //iterate over all pixels in the region
    CRTSettings* settings = scene->getSettings();
    for(int y = 0; y < regionHeight;y++) {
        for(int x = 0; x < regionWidth;x++) { 
            int actualY = startY+y;
            int actualX = startX+x;
            CRTRay cameraRay = scene->sceneCamera.generateCameraRay(actualY, actualX);
            cameraRay.rayDepth = 0;
            cameraRay.type = CameraRay;
            CRTVector color = traceRay(cameraRay);
            image.setPixel(color,actualX,actualY);
            //CRTVector final_color = CRTVector(glm::clamp(color.x,0.f,1.f),glm::clamp(color.y,0.f,1.f),glm::clamp(color.z,0.f,1.f));
            //image.image[y][x] = {(int) (final_color.x*255.f),(int) (final_color.y*255.f),(int) (final_color.z*255.f)};
        }
    }
}

CRTVector CRTRenderer::traceRay(const CRTRay& ray, const float maxT) {
    if(ray.rayDepth >= maxDepth) return scene->getBackgroundColor();

    Intersection isect;
    if(as.findIntersection(ray, isect)) {
        //Intersection isect2;
        //as.findIntersection(ray, isect2);
        //std::cout << "test" << std::endl;
        Material mat = scene->sceneMaterials[isect.materialIDx];
        return calculateShading(ray, isect);

    } else {
        return scene->sceneSettings.backgroundColor;
    }

}
bool CRTRenderer::intersect(const CRTRay& ray,Intersection& isect, const float maxT) {
    bool foundIntersection = false;
    float closestIntersectionDistance = FLT_MAX;
    int materialID;
    int objectID;
    int triangleID;
    int textureID{0};
    CRTVector geoNormal;
    CRTVector shadingNormal;
    CRTVector baryCoords;
    CRTVector position;
    CRTVector textureCoords{0.f};
    if(!CRTRay::intersectBoundingBox(ray, entireSceneBB)) {
        return false;
    }
    for(int i = 0; i < scene->sceneObjects.size(); i++) {
        CRTMesh* object = &(scene->sceneObjects[i]);
        for(int k = 0; k < object->triangleVertIndices.size();k+=3) {
            CRTTriangle triangle(object->triangleVertices[object->triangleVertIndices[k]],
                                object->triangleVertices[object->triangleVertIndices[k+1]],
                                object->triangleVertices[object->triangleVertIndices[k+2]]);
            float t;
            bool hitCondition = false;

            //shoot shadowRay
            if(ray.type == ShadowRay ||ray.type == RefractionRay ||ray.type == ReflectionRay){
                hitCondition = CRTRay::intersectTriangle(ray,triangle,t, true) && t < closestIntersectionDistance && t < maxT;
            } else {
                hitCondition = CRTRay::intersectTriangle(ray,triangle,t, false) && t < closestIntersectionDistance && t < maxT;
            }
            //if(ray.type == CameraRay) {
                
            //}

            if(hitCondition) {

                foundIntersection = true;
                closestIntersectionDistance = t;
                materialID = object->materialID;
                objectID = i;
                triangleID = k;
                geoNormal = triangle.normal;
                position = ray.rayOrigin + ray.rayDirection*t;
                baryCoords = CRTTriangle::calculateBarycentricCoordinates(triangle,position);
                shadingNormal = object->vertexNormals[object->triangleVertIndices[k]]*baryCoords.z +object->vertexNormals[object->triangleVertIndices[k+1]]*baryCoords.x + object->vertexNormals[object->triangleVertIndices[k+2]]*baryCoords.y;

                
                Material mat = scene->getMaterial(materialID);

                /*
                for(int j =0; j < scene->sceneTextures.size();j++) {
                    if(!mat.albedoTex.std::string::compare(scene->sceneTextures[j].name)) {
                        textureID = j;
                        break;
                    }
                }
                if(scene->sceneTextures[textureID].type == checkersTexture || scene->sceneTextures[textureID].type == bitmapTexture) {
                    textureCoords = object->textureCoords[object->triangleVertIndices[k]]*baryCoords.z +object->textureCoords[object->triangleVertIndices[k+1]]*baryCoords.x + object->textureCoords[object->triangleVertIndices[k+2]]*baryCoords.y;
                }
                    */
                if(ray.type == ShadowRay) {
                    Material mat = scene->getMaterial(materialID);
                    if (mat.type != refractive) {
                        return true;
                    } else {
                        foundIntersection = false;
                        continue;
                    }
                }
            }
        }
    }
    if(foundIntersection) {
        isect.intersectionPoint = position;
        isect.baryCoords = baryCoords;
        isect.geomNormal = geoNormal;
        isect.shadingNormal = shadingNormal;
        isect.materialIDx = materialID;
        isect.objectIDx = objectID;
        isect.triangleIDx = triangleID;
        isect.t = closestIntersectionDistance;
    }
    return foundIntersection;
}

CRTVector CRTRenderer::calculateShading(const CRTRay& ray,Intersection& isect) {
    Material mat = scene->sceneMaterials[isect.materialIDx];
    
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
    return scene->sceneMaterials[isect.materialIDx].albedo;
}

CRTVector CRTRenderer::diffuseShading(const CRTRay& ray,Intersection& isect ) {
    Material mat = scene->getMaterial(isect.materialIDx);
    //
    CRTVector final_color(0.f);

    //get the albedo of the material 
    CRTVector albedo = mat.albedo;
    //CRTVector albedo = aTexture.sample(isect.baryCoords);

    CRTVector normal = mat.style == flat ? isect.geomNormal : isect.shadingNormal;

    for(Light source : scene->sceneLights) {
        //determine vector to light source from intersectionPoint
        CRTVector lD = (source.lightPosition - isect.intersectionPoint);
        //adjust the shadow ray origin in direction of the triangle normal to avoid self-intersection and shadow acne
        const CRTVector shadowRayOrigin = isect.intersectionPoint + normal * shadowbias;
        CRTRay shadowRay(shadowRayOrigin,lD.normalize());
        shadowRay.type= ShadowRay;
        Intersection shadowIsect;
        if(intersect(shadowRay, shadowIsect,lD.length())) {
            continue;
        }
        float lDLength = lD.length();
        float cosLaw = std::max(0.f,CRTVector::dot(lD.normalize(), normal));
        if(cosLaw ==0.f) continue;
        float distanceFallOff = 4*M_PI*lDLength*lDLength;
        final_color = final_color +(albedo*(cosLaw*source.lightIntensity/distanceFallOff));
        }
    //final_color = CRTVector(glm::clamp(final_color.x,0.f,1.f),glm::clamp(final_color.y,0.f,1.f),glm::clamp(final_color.z,0.f,1.f));

    return final_color;
}

CRTVector CRTRenderer::reflectiveShading(const CRTRay& ray,Intersection& isect) {
    Material mat = scene->sceneMaterials[isect.materialIDx];
    CRTVector normal;
    if(mat.style == smooth) {
        normal = isect.shadingNormal;
    } else {
        normal = isect.geomNormal;
    }           
    CRTVector shadingResult = traceRay(createReflectionRay(ray, isect.intersectionPoint, normal));
    return CRTVector(mat.albedo.x*shadingResult.x,mat.albedo.y*shadingResult.y,mat.albedo.z*shadingResult.z);
}


CRTVector CRTRenderer::refractiveShading(const CRTRay& ray,Intersection& isect) {
    
    Material mat = scene->getMaterial(isect.materialIDx);
    CRTVector normal;
    if(mat.style == smooth) {
        normal = isect.shadingNormal;
    } else {
        normal = isect.geomNormal;
    } 
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
    if(sqrt(1-cosAlpha*cosAlpha)> exitIOR/entryIOR) {
        return traceRay(createReflectionRay(ray, isect.intersectionPoint, normal));
    }
    float sinBeta = sqrt(1-cosAlpha*cosAlpha) *entryIOR / exitIOR;
    float cosBeta = sqrt(1-sinBeta*sinBeta);
    CRTVector C = (I +  cosAlpha*normal).normalize();
    CRTVector B = C*sinBeta;
    CRTVector A = cosBeta * -1.f * normal;
    CRTVector R = A +B;
    CRTRay refractionRay(isect.intersectionPoint + -1.f*0.001f*normal,R);
    //glm::refract(ray.rayDirection)
    refractionRay.rayDepth = ray.rayDepth+1;
    refractionRay.type = RefractionRay;
    float f = fresnel(ray,normal);
    return f * traceRay(createReflectionRay(ray, isect.intersectionPoint, normal)) +  (1.f-f)* traceRay(refractionRay);
}
CRTRay CRTRenderer::createReflectionRay(const CRTRay& ray, const CRTVector& position, const CRTVector& normal) {
    const CRTVector reflectionOrigin = position + normal * reflectionBias;
    CRTRay reflectionRay(reflectionOrigin,CRTRay::reflect(ray.rayDirection, normal));
    reflectionRay.type = ReflectionRay;
    reflectionRay.rayDepth = ray.rayDepth+1;
    return reflectionRay;
}

CRTVector refract(const CRTRay& ray,const CRTVector normal, const float relativeIOR) {
    return CRTVector{0.f};
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

void CRTRenderer::generateBoundingBox() {
    for(CRTMesh object : scene->sceneObjects) {
        for(int i = 0; i < object.triangleVertIndices.size();i+=3) {
            CRTTriangle triangle(object.triangleVertices[object.triangleVertIndices[i]],
                            object.triangleVertices[object.triangleVertIndices[i+1]],
                            object.triangleVertices[object.triangleVertIndices[i+2]]);

            entireSceneBB.include(triangle);
        }
    }
}


float CRTRenderer::fade(float low, float high, float value) {
    float mid = (low+high)*0.5f;
    float range = (high-low)*0.5f;
    float x = 1.f-std::clamp(std::abs(mid-value)/range,0.f,1.f);
    return std::lerp(0.f,1.f,x);
}

CRTVector CRTRenderer::temperature(float intensity) {
    const CRTVector blue(0.f,0.f,1.f);
    const CRTVector cyan(0.f,1.f,1.f);
    const CRTVector green(0.f,1.f,0.f);
    const CRTVector yellow(1.f,1.f,0.f);
    const CRTVector red(1.f,0.f,0.f);
    return CRTVector(fade(-0.25f, 0.25f, intensity)*blue
                        +fade(0.f, 0.5f, intensity)*cyan
                        +fade(0.25f, 0.75f, intensity)*green
                        +fade(0.5f, 1.f, intensity)*yellow
                        +std::lerp(0.75f, 1.f, intensity)*red);
}