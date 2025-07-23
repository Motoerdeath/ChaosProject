#ifndef CRTBUCKET
#define CRTBUCKET

#include <queue>

struct Bucket {
    int bucketIDx;
    int startX, startY;
    int width,height;
};

class BucketQueue {
    public:
    BucketQueue() {}
    void generateBucketQueue(int imageWidth, int imageHeight, int bucketSize);
    void generateRegionQueue(int imageWidth, int imageHeight, int numThreads);
    std::queue<Bucket> buckets;
    private:
    

};

#endif