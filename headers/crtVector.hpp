#ifndef CRTVECTOR
#define CRTVECTOR

#include <cmath>

//3 dimensional vectorsused for position and direction data
class CRTVector2 {

    public:

    CRTVector2(float x, float y, float z) : x{x}, y{y}, z{z} {}
    CRTVector2(float all) : x{all}, y{all}, z{all} {}
    float length() const {
        return sqrtf((x*x+y*y+z*z));
    }
    CRTVector2 normalize() const {
        float len = length();
        return CRTVector2(x/len,y/len,z/len);
    }
    CRTVector2 operator+ (const CRTVector2& lhs) const {
        return CRTVector2(x+lhs.x,y+lhs.y,z+lhs.z);
    }
    CRTVector2 operator- (const CRTVector2& lhs) const {
        return CRTVector2(x-lhs.x,y-lhs.y,z-lhs.z);
    }
    CRTVector2 cross(const CRTVector2& lhs) const {

    }  
      
    float x;
    float y;
    float z;

    private:


};

#endif