#ifndef CRTBUCKET
#define CRTBUCKET

#include <queue>

struct Bucket {
    int x,y,width,height;
};

class BucketQueue {
    public:
    BucketQueue() {}
    

    private:
    std::queue<Bucket> buckets;

};

#endif