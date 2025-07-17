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
    std::queue<Bucket> buckets;
    private:
    

};

#endif