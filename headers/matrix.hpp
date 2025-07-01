#ifndef CRTMATRIX
#define CRTMATRIX
#include "crtVector.hpp"
#include <cmath>
#define M_PI 3.14159265358979323846
#include <complex>
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
    float getElement(int row, int column) const {
        return matrix[row][column];
    }
    CRTMatrix static getRotationMatrixAroundX(float degs) {
        const float rads = degs*(M_PI/180.f);
        return CRTMatrix(
            CRTVector(1.0f,0.0f,0.0f),
            CRTVector(0.0f,std::cos(rads),-std::sin(rads)),
            CRTVector(0.0f,std::sin(rads),std::cos(rads))
        );
    }
    CRTMatrix static getRotationMatrixAroundY(float degs) {
        const float rads = degs*(M_PI/180.f);
        return CRTMatrix(
            CRTVector(cos(rads),0.0f,-std::sin(rads)),
            CRTVector(0.0f,1.0f,0.0f),
            CRTVector(std::sin(rads),0.0f,std::cos(rads))
        );
    }
    CRTMatrix static getRotationMatrixAroundZ(float degs) {
        const float rads = degs*(M_PI/180.f);
        return CRTMatrix(
            CRTVector(cos(rads),-std::sin(rads),0.0f),
            CRTVector(sin(rads),std::cos(rads),0.0f),
            CRTVector(0.0f,0.0f,1.0f)
        );
    }
    CRTMatrix transform(CRTVector movDir);
    CRTMatrix scale(float scalar);

    CRTMatrix operator*(const float scalar) {
        return CRTMatrix(CRTVector(matrix[0][0]*scalar,matrix[0][1]*scalar,matrix[0][2]*scalar),
        CRTVector(matrix[1][0]*scalar,matrix[1][1]*scalar,matrix[1][2]*scalar),
        CRTVector(matrix[2][0]*scalar,matrix[2][1]*scalar,matrix[2][2]*scalar));
    }
    friend CRTVector operator*(const CRTMatrix& lhs, const CRTVector& rhs) {
            return CRTVector(lhs.getElement(0, 0)*rhs.x+lhs.getElement(0, 1)*rhs.y+lhs.getElement(0, 2)*rhs.z,
                lhs.getElement(1, 0)*rhs.x+lhs.getElement(1, 1)*rhs.y+lhs.getElement(1, 2)*rhs.z,
                lhs.getElement(2, 0)*rhs.x+lhs.getElement(2, 1)*rhs.y+lhs.getElement(2, 2)*rhs.z
            );
    }
    /*
    CRTVector operator*(const CRTVector lhs, const CRTMatrix rhs) const {
    return 
        CRTVector(lhs*rhs.getElement(0, 0) + y *rhs.getElement(1, 0) + z *rhs.getElement(2, 0),
                x*rhs.getElement(0, 1) + y *rhs.getElement(1, 1) + z *rhs.getElement(2, 1),
                x*rhs.getElement(0, 2) + y *rhs.getElement(1, 2) + z *rhs.getElement(2, 2)
    
    );
    }
    */
    CRTMatrix operator*(const CRTMatrix rhs) {
        CRTVector v0(
            matrix[0][0]*rhs.getElement(0,0)+matrix[0][1]*rhs.getElement(1,0)+matrix[0][2]*rhs.getElement(2,0),
            matrix[0][0]*rhs.getElement(0,1)+matrix[0][1]*rhs.getElement(1,1)+matrix[0][2]*rhs.getElement(2,1),
            matrix[0][0]*rhs.getElement(0,2)+matrix[0][1]*rhs.getElement(1,2)+matrix[0][2]*rhs.getElement(2,2)
        );
        CRTVector v1(
            matrix[1][0]*rhs.getElement(0,0)+matrix[1][1]*rhs.getElement(1,0)+matrix[1][2]*rhs.getElement(2,0),
            matrix[1][0]*rhs.getElement(0,1)+matrix[1][1]*rhs.getElement(1,1)+matrix[1][2]*rhs.getElement(2,1),
            matrix[1][0]*rhs.getElement(0,2)+matrix[1][1]*rhs.getElement(1,2)+matrix[1][2]*rhs.getElement(2,2)
        );
        CRTVector v2(
            matrix[2][0]*rhs.getElement(0,0)+matrix[2][1]*rhs.getElement(1,0)+matrix[2][2]*rhs.getElement(2,0),
            matrix[2][0]*rhs.getElement(0,1)+matrix[2][1]*rhs.getElement(1,1)+matrix[2][2]*rhs.getElement(2,1),
            matrix[2][0]*rhs.getElement(0,2)+matrix[2][1]*rhs.getElement(1,2)+matrix[2][2]*rhs.getElement(2,2)
        );

        return CRTMatrix(v0,v1,v2);
            
    }
    friend CRTVector operator*(const CRTVector& lhs, const CRTMatrix& rhs) {
        return 
        CRTVector(lhs.x*rhs.getElement(0, 0) + lhs.y *rhs.getElement(1, 0) + lhs.z *rhs.getElement(2, 0),
                lhs.x*rhs.getElement(0, 1) + lhs.y *rhs.getElement(1, 1) + lhs.z *rhs.getElement(2, 1),
                lhs.x*rhs.getElement(0, 2) + lhs.y *rhs.getElement(1, 2) + lhs.z *rhs.getElement(2, 2));
    };
    private:
    float matrix[3][3];
};

#endif