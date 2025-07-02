#include "../headers/scene.hpp"
#include "../headers/triangle.hpp"

void CRTScene::render() {

    for(int i = 0; i < settings.imageHeight;i++) {
        for(int j = 0; j < settings.imageWidth;j++) {
            float closestIntersectionDistance = FLT_MAX;
            for(CRTMesh object : objects) {
                for(int i = 0; i < object.triangleVertIndices.size();i+=3) {
                    int triangleFirstIndex = object.triangleVertIndices[i];
                    CRTTriangle triangle(object.triangleSoup[triangleFirstIndex],
                                        object.triangleSoup[triangleFirstIndex+1],
                                        object.triangleSoup[triangleFirstIndex+2]);

                    float t = 1.f;
                    CRTRay ray = camera.generateCameraRay(i, j);

                    if(ray.intersectTriangle(triangle, t)) {
                        if(t < closestIntersectionDistance) {
                            closestIntersectionDistance = t;
                            image.setPixel(triangle.color[0], triangle.color[1], triangle.color[2], j, i);
                        }
                    } else {
                        image.setPixel((int)(image.backgroundColor.x *255.f), (int)(image.backgroundColor.y *255.f), (int)(image.backgroundColor.z *255.f), j, i);
                    }             
                }
            }
        }
    }
}

/*
        for(int i = 0; i < height;i++) {
            for(int j = 0; j < width;j++) {
                float lowestDistance = FLT_MAX;
                for(CRTTriangle triangle : triangles) {
                    float t = 1.f;
                    CRTRay ray = camera.generateCameraRay(i, j);
                    if(ray.intersectTriangle(triangle, t)) {
                        if(t < lowestDistance) {
                            lowestDistance = t;
                            image.setPixel(triangle.color[0], triangle.color[1], triangle.color[2], j, i);
                        }
                    } else {
                        image.setPixel((int)(image.backgroundColor.x *255.f), (int)(image.backgroundColor.y *255.f), (int)(image.backgroundColor.z *255.f), j, i);
                    }
                }
            }
        }
            */