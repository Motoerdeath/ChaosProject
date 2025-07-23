#include <queue>
#include "../headers/bucket.hpp"
#include <cmath>
std::queue<Bucket> buckets;


void BucketQueue::generateBucketQueue(int imageWidth, int imageHeight, int bucketSize) {
    
    int bucketsX = std::ceil(static_cast<float>(imageWidth)/static_cast<float>(bucketSize));
    int bucketsY = std::ceil(static_cast<float>(imageHeight)/static_cast<float>(bucketSize));
    for(int i = 0; i < bucketsY;i++) {
        for(int j = 0; j < bucketsX; j++) {
            Bucket newBucket{i*bucketsX+j,bucketSize*j,bucketSize*i,bucketSize,bucketSize};
            buckets.push(newBucket);
        }
    }
}

void BucketQueue::generateRegionQueue(int imageWidth, int imageHeight, int numThreads) {
    
    int regionsX = std::floor(sqrt(numThreads));
    int regionsY = std::floor(sqrt(numThreads));
    int regionSizeX = imageWidth / regionsX;
    int regionSizeY = imageHeight / regionsY;

    for(int i = 0; i < regionsY;i++) {
        for(int j = 0; j < regionsX; j++) {
            Bucket newBucket{i*regionsX+j,regionSizeX*j,regionSizeY*i,regionSizeX,regionSizeY};
            buckets.push(newBucket);
        }
    }
}