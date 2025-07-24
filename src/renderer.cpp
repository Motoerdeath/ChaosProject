#include "../headers/renderer.hpp"
#include "../include/glm/geometric.hpp"
#include <algorithm>
#include <cfloat>
#include <cmath>
#include <iostream>
#include <random>
#include <stdexcept>
#include <chrono>
#include <thread>
#include "../headers/globalSettings.hpp"


CRTRenderer::CRTRenderer(CRTScene* scene) : scene(scene){
    scene2 = std::unique_ptr<CRTScene>(scene);
    CRTSettings* settings = scene->getSettings();
    image = PPMImage(settings->imageWidth,settings->imageHeight,255.f);
};
void CRTRenderer::setupTriangleAccessStructure() {
    if(MOVABLEOBJECTS) {
        as.createTriangleSoup(scene->fullObjects);
    } else {
        as.createTriangleSoup(scene->sceneObjects);
    }
    
    
    as.buildAS();
}
void CRTRenderer::render() {
    if(MULTITHREADING) {
        renderMultiThreaded();
    } else {
        renderSingleThreaded();
    }

}
void CRTRenderer::renderRegion(const int startX,const int startY,const int regionWidth, const int regionHeight) {

    //iterate over all pixels in the region
    CRTSettings* settings = scene->getSettings();
    for(int y = 0; y < regionHeight;y++) {
        for(int x = 0; x < regionWidth;x++) { 
            int actualY = startY+y;
            int actualX = startX+x;
            auto start = std::chrono::steady_clock::now();//timing
            CRTVector finalColor{0.f};

            for(int i = 0; i < SAMPLESPERPIXEL; i++) {
                CRTRay cameraRay = scene->sceneCamera.generateCameraRay(actualY, actualX,CAMERAJITTER);
                cameraRay.rayDepth = 0;
                cameraRay.type = CameraRay;
                finalColor = finalColor + traceRay(cameraRay);

                if(debug == HeatMap) {
                    auto finish = std::chrono::steady_clock::now();
                    const std::chrono::duration<float> elapsed_seconds{finish - start};
                    float time = glm::clamp(elapsed_seconds.count()/heatMapHigh,0.f,1.f);//(elapsed_seconds.count()/heatMapHigh,0.f,1.f);
                    finalColor =  finalColor +temperature(time);
                }
            }
            finalColor = finalColor/SAMPLESPERPIXEL;
            image.setPixel(finalColor,actualX,actualY);

        }
    }
}

CRTVector CRTRenderer::traceRay(const CRTRay& ray, const float maxT) {
    if(ray.rayDepth >= MAXPATHDEPTH) return scene->getBackgroundColor();

    Intersection isect;

    if(ACCELERATION) {
        if(as.findIntersection(ray, isect)) {
            Material mat = scene->sceneMaterials[isect.materialIDx];
            return calculateShading(ray, isect);

        } else {
            return scene->sceneSettings.backgroundColor;
        }
    } else {
        if(MOVABLEOBJECTS) {
            if(intersect2(ray, isect)) {
                Material mat = scene->sceneMaterials[isect.materialIDx];
                return calculateShading(ray, isect);

            } else {
                return scene->sceneSettings.backgroundColor;
            }
        } else {
            if(intersect(ray, isect)) {
                Material mat = scene->sceneMaterials[isect.materialIDx];
                return calculateShading(ray, isect);

            } else {
                return scene->sceneSettings.backgroundColor;
            }
        }

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
    for(int i = 0; i < scene->sceneObjects.size(); i++) {
        CRTMesh* object = &(scene->sceneObjects[i]);
        for(int k = 0; k < object->triangleVertIndices.size();k+=3) {
            CRTTriangle triangle(object->triangleVertices[object->triangleVertIndices[k]],
                                object->triangleVertices[object->triangleVertIndices[k+1]],
                                object->triangleVertices[object->triangleVertIndices[k+2]]);
            float t;
            bool hitCondition = false;
            Material mat = scene->getMaterial(object->materialID);
            bool hitBackSide = !mat.backFaceCulling;
            //shoot shadowRay
            hitCondition = CRTRay::intersectTriangle(ray,triangle,t, hitBackSide) && t < closestIntersectionDistance && t < maxT;

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
bool CRTRenderer::intersect2(const CRTRay& ray,Intersection& isect, const float maxT) {
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
    for(int i = 0; i < scene->fullObjects.size(); i++) {
        CRTObject fullObject = scene->fullObjects[i];
        CRTMesh* object = &fullObject.mesh;
        for(int k = 0; k < object->triangleVertIndices.size();k+=3) {
            CRTTriangle triangle(object->triangleVertices[object->triangleVertIndices[k]]+fullObject.offset,
                                object->triangleVertices[object->triangleVertIndices[k+1]]+fullObject.offset,
                                object->triangleVertices[object->triangleVertIndices[k+2]]+fullObject.offset);
            float t;
            bool hitCondition = false;
            Material mat = scene->getMaterial(object->materialID);
            bool hitBackSide = !mat.backFaceCulling;
            //shoot shadowRay
            hitCondition = CRTRay::intersectTriangle(ray,triangle,t, hitBackSide) && t < closestIntersectionDistance && t < maxT;

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
    
    if(debug == GeometricNormals){
        return isect.geomNormal;
    } else if(debug == ShadingNormals) {
        return isect.shadingNormal;
    } else if(debug == BarycentricCoordinates) {
        return CRTVector(isect.baryCoords.x,isect.baryCoords.y,0.f);
    } else if(debug == TextureCoordinates) {
        return isect.textureCoords;
    } else if (debug == TriangleView) {

    } else {
        if(ray.rayDepth>= MAXPATHDEPTH) {
            return scene->getBackgroundColor();  
        } else if(mat.type == constant) {
            return constantShading(ray, isect);
        } else if(mat.type == diffuse) {
            if(GLOBALILLUMINATION) {
                return diffuseShadingGI(ray, isect);
            } else {
                return diffuseShading(ray, isect);
            }
        } else if(mat.type == reflective) {
            return reflectiveShading(ray, isect);
        } else if(mat.type == refractive) {
            return refractiveShading(ray, isect);
        } else {
            throw std::runtime_error("invalid Rendering Style");
        }
        
        return CRTVector(0.f);
    }

}

CRTVector CRTRenderer::getAlbedo(Material mat,Intersection& isect)  {
    if(!mat.albedoTex.std::string::compare("invalid")) {
        return mat.albedo;
    } else {
        for(Texture tex : scene->sceneTextures) {
            if(!tex.name.std::string::compare(mat.albedoTex)) {
                if(tex.type == checkersTexture || tex.type == bitmapTexture) {
                    return tex.sample(isect.textureCoords);
                } else {
                    return tex.sample(isect.baryCoords);
                }
            }
        }
    }
    return CRTVector(0.f);
}
CRTVector CRTRenderer::constantShading(const CRTRay& ray,Intersection& isect) {
    Material mat = scene->sceneMaterials[isect.materialIDx];
    return getAlbedo(mat, isect);
    return scene->sceneMaterials[isect.materialIDx].albedo;
}

CRTVector CRTRenderer::diffuseShading(const CRTRay& ray,Intersection& isect ) {
    Material mat = scene->getMaterial(isect.materialIDx);
    //
    CRTVector final_color(0.f);

    //get the albedo of the material 
    CRTVector albedo = getAlbedo(mat, isect);

    CRTVector normal = mat.style == flat ? isect.geomNormal : isect.shadingNormal;

    for(Light source : scene->sceneLights) {
        //determine vector to light source from intersectionPoint
        CRTVector lD = (source.lightPosition - isect.intersectionPoint);
        //adjust the shadow ray origin in direction of the triangle normal to avoid self-intersection and shadow acne
        const CRTVector shadowRayOrigin = isect.intersectionPoint + normal * shadowbias;
        CRTRay shadowRay(shadowRayOrigin,lD.normalize());
        shadowRay.type= ShadowRay;
        Intersection shadowIsect;
        if(as.findIntersection(shadowRay, shadowIsect,lD.length())) {
            Material shadowMat = scene->getMaterial(shadowIsect.materialIDx);
            if(shadowMat.type != refractive) continue;
            //continue;
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

CRTVector CRTRenderer::diffuseShadingGI(const CRTRay& ray,Intersection& isect){
    Material mat = scene->getMaterial(isect.materialIDx);
    CRTVector normal = mat.style == flat ? isect.geomNormal : isect.shadingNormal;
    CRTVector final_color(0.f);
    int diffuseReflectionRaysCount = 1;
    for(int i = 0; i < diffuseReflectionRaysCount;i++) {
        //construct local hit matrix
        CRTVector rightAxis = ray.rayDirection.cross(normal).normalize();
        CRTVector upAxis = normal;
        CRTVector forwardAxis = rightAxis.cross(upAxis);
        CRTMatrix localHitMatrix{rightAxis,upAxis,forwardAxis};
        float randAngleinXY = M_PI*dist(mt);
        CRTVector randVectorInXY{std::cos(randAngleinXY),std::sin(randAngleinXY),0.f};
        float randAngleinXZ = 2.f*M_PI*dist(mt);
        CRTMatrix rotateAroundY = CRTMatrix::getRotationMatrixAroundY(randAngleinXZ);
        CRTVector randVectorInXYRotated = randVectorInXY*rotateAroundY;
        CRTVector diffReflRayDir = randVectorInXYRotated*localHitMatrix;
        CRTRay diffReflRay(isect.intersectionPoint + normal*0.001f,diffReflRayDir);
        diffReflRay.type = CameraRay;
        diffReflRay.rayDepth = ray.rayDepth+1;
        final_color = final_color + traceRay(diffReflRay);
    }
    
    final_color = final_color + diffuseShading(ray, isect);
    return final_color / (diffuseReflectionRaysCount+1);
    

    /*
    finalCOlor = finalColor /diffuseReflectionRaysCount;
    finalColor = (finalColor + diffuseShading) * getAlbedo(mat,isect)/M_PI;
    
    */
}

CRTVector CRTRenderer::reflectiveShading(const CRTRay& ray,Intersection& isect) {
    Material mat = scene->sceneMaterials[isect.materialIDx];
    CRTVector albedo = getAlbedo(mat, isect);
    CRTVector normal;
    if(mat.style == smooth) {
        normal = isect.shadingNormal;
    } else {
        normal = isect.geomNormal;
    }           
    CRTVector shadingResult = traceRay(createReflectionRay(ray, isect.intersectionPoint, normal));
    return CRTVector(albedo.x*shadingResult.x,albedo.y*shadingResult.y,albedo.z*shadingResult.z);
}


CRTVector CRTRenderer::refractiveShading(const CRTRay& ray,Intersection& isect) {
    
    Material mat = scene->getMaterial(isect.materialIDx);
    CRTVector albedo = getAlbedo(mat, isect);
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
    CRTVector combinedTerm = f * traceRay(createReflectionRay(ray, isect.intersectionPoint, normal)) +  (1.f-f)* traceRay(refractionRay);
    return CRTVector(albedo.x*combinedTerm.x,albedo.y*combinedTerm.y,albedo.z*combinedTerm.z);
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

void CRTRenderer::renderSingleThreaded() {
    CRTSettings* settings = scene->getSettings();
    for(int y = 0; y < settings->imageHeight;y++) {
        for(int x = 0; x < settings->imageWidth;x++) { 


            auto start = std::chrono::steady_clock::now();//timing
            CRTRay cameraRay = scene->sceneCamera.generateCameraRay(y, x,CAMERAJITTER);
            cameraRay.rayDepth = 0;
            cameraRay.type = CameraRay;
            CRTVector color = traceRay(cameraRay);

            if(debug == HeatMap) {
                auto finish = std::chrono::steady_clock::now();
                const std::chrono::duration<float> elapsed_seconds{finish - start};
                float time = glm::clamp(elapsed_seconds.count()*heatMapHigh,0.f,1.f);//(elapsed_seconds.count()/heatMapHigh,0.f,1.f);
                color = temperature(time);
            }
            //Heatmap and timing stuff
            image.setPixel(color,x,y);       
        }
        std::cout << "row:" << y+1 <<"/" <<settings->imageHeight << " finished" << std::endl;
    }
}
void threadFunc(std::queue<Bucket>* buckets, CRTRenderer* renderer) {
  //
  while(true) {
    renderer->bucketMutex.lock();
    if(buckets->size() > 0) {
      Bucket temp = buckets->front();
      buckets->pop();
      //std::cout << "Thread " << threadIndex<<" acquired lock on value " << temp.width<<std::endl;
      renderer->bucketMutex.unlock();
      //std::this_thread::sleep_for(std::chrono::milliseconds(100));
      renderer->renderRegion(temp.startX, temp.startY, temp.width, temp.height);
      //std::cout << "Bucket " << temp.bucketIDx<<" finished!"<<std::endl;

      /* //get updates for when a bucket has been finished and how many
      renderer->updateMutex.lock();
      renderer->finishedBuckets++;
      std::cout << renderer->finishedBuckets<<" Buckets finished!"<<std::endl;
      renderer->updateMutex.unlock();
      */
    } else {
      renderer->bucketMutex.unlock();
      return;
    }

  }

  //

  return;
}

void CRTRenderer::renderMultiThreaded() {
    CRTSettings* settings = scene->getSettings();
    renderQueue.generateBucketQueue(settings->imageWidth, settings->imageHeight, 24);
    const auto nThreads = std::thread::hardware_concurrency();

    std::vector<std::thread> threads;
    //renderQueue.generateRegionQueue(settings->imageWidth, settings->imageHeight, nThreads);
    for(int i = 0; i < nThreads;i++) {
        threads.push_back(std::thread(threadFunc,&renderQueue.buckets,this));
        //threads.push_back(std::thread(&threadFunc));
    }
        

    for(std::thread& t : threads) {
        t.join();
    }
}

void CRTRenderer::setupRNG() {

    std::random_device rd;
    mt = std::mt19937(rd());
    dist = std::uniform_real_distribution<float>(0.f,1.f);

}