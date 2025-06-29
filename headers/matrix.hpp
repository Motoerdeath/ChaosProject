#ifndef CRTMACRO
#define CRTMACRO
#include "crtVector.hpp"
#include <cmath>
class CRTMatrix{
    public:
    CRTMatrix(CRTVector vector1,CRTVector vector2,CRTVector vector3) {
        matrix[0][0] = vector1.x;
        matrix[0][1] = vector1.y;
        matrix[0][2] = vector1.z;
        matrix[1][0] = vector2.x;
        matrix[1][1] = vector2.y;
        matrix[1][2] = vector2.z;
        matrix[2][0] = vector3.x;
        matrix[2][1] = vector3.y;
        matrix[2][2] = vector3.z;
    }
    CRTMatrix rotate(float ang) {

        //return this*getRotationMatrixAroundX( ang);
    }
    CRTMatrix static getRotationMatrixAroundX(float ang) {
        return CRTMatrix(
            CRTVector(1.0f,0.0f,0.0f),
            CRTVector(0.0f,std::cos(ang),-std::sin(ang)),
            CRTVector(0.0f,std::sin(ang),std::cos(ang))
        );
    }
    CRTMatrix static getRotationMatrixAroundY(float ang) {
        return CRTMatrix(
            CRTVector(cos(ang),0.0f,-std::sin(ang)),
            CRTVector(0.0f,1.0f,0.0f),
            CRTVector(std::sin(ang),0.0f,std::cos(ang))
        );
    }
    CRTMatrix static getRotationMatrixAroundZ(float ang) {
        return CRTMatrix(
            CRTVector(cos(ang),-std::sin(ang),0.0f),
            CRTVector(sin(ang),std::cos(ang),0.0f),
            CRTVector(0.0f,0.0f,1.0f)
        );
    }
    CRTMatrix transform(CRTVector movDir);
    CRTMatrix scale(float scalar);

    CRTMatrix operator*(const float scalar);
    CRTMatrix operator*(const CRTVector vector);
    CRTMatrix operator*(const CRTMatrix matrix);
    float matrix[3][3];
    private:
    
};

#endif