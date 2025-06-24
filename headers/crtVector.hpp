#ifndef CRTVECTOR
#define CRTVECTOR

#include <cmath>

//3 dimensional vectorsused for position and direction data
class CRTVector {

    public:

    CRTVector(float x, float y, float z) : x{x}, y{y}, z{z} {}
    CRTVector(float all) : x{all}, y{all}, z{all} {}
    float length() const {
        return sqrtf((x*x+y*y+z*z));
    }
    CRTVector normalize() const {
        float len = length();
        return CRTVector(x/len,y/len,z/len);
    }
    CRTVector operator+ (const CRTVector& lhs) const {
        return CRTVector(x+lhs.x,y+lhs.y,z+lhs.z);
    }
    CRTVector operator- (const CRTVector& lhs) const {
        return CRTVector(x-lhs.x,y-lhs.y,z-lhs.z);
    }
    CRTVector cross(const CRTVector& lhs) const {

    }  

    float x;
    float y;
    float z;

    private:


};

#endif