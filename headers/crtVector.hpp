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
    CRTVector operator+ (const CRTVector& rhs) const {
        return CRTVector(x+rhs.x,y+rhs.y,z+rhs.z);
    }
    CRTVector operator- (const CRTVector& rhs) const {
        return CRTVector(x-rhs.x,y-rhs.y,z-rhs.z);
    }
    CRTVector cross(const CRTVector& rhs) const {
        return CRTVector(y*rhs.z-z*rhs.y,
                         z*rhs.x-x*rhs.z,
                         x*rhs.y-y*rhs.x);
    }  

    float x;
    float y;
    float z;

    private:


};

#endif