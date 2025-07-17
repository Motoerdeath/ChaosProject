#include <queue>
#include "../headers/bucket.hpp"
#include <cmath>
std::queue<Bucket> buckets;


void BucketQueue::generateBucketQueue(int imageWidth, int imageHeight, int bucketSize) {
    
    int bucketsX = std::ceil(static_cast<float>(imageWidth)/static_cast<float>(bucketSize));
    int bucketsY = std::ceil(static_cast<float>(imageHeight)/static_cast<float>(bucketSize));
    for(int i = 0; i < bucketsY;i++) {
        for(int j = 0; j < bucketsX; j++) {
            Bucket newBucket{i*bucketsX+j,bucketSize*j,bucketSize*i,24,24};
            buckets.push(newBucket);
        }
    }
}