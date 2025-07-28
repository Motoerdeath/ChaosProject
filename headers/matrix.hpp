#ifndef CRTMATRIX
#define CRTMATRIX
#include "crtVector.hpp"
#include <cmath>
#define M_PI 3.14159265358979323846
#include <complex>
class CRTMatrix{
    public:
    CRTMatrix();
    CRTMatrix(CRTVector vector1,CRTVector vector2,CRTVector vector3);
    float getElement(int row, int column) const ;
    CRTMatrix static getRotationMatrixAroundX(float degs);
    CRTMatrix static getRotationMatrixAroundY(float degs);
    CRTMatrix static getRotationMatrixAroundZ(float degs);
    CRTMatrix operator*(const float scalar);
    friend CRTVector operator*(const CRTMatrix& lhs, const CRTVector& rhs) {
            return CRTVector(lhs.getElement(0, 0)*rhs.x+lhs.getElement(0, 1)*rhs.y+lhs.getElement(0, 2)*rhs.z,
                lhs.getElement(1, 0)*rhs.x+lhs.getElement(1, 1)*rhs.y+lhs.getElement(1, 2)*rhs.z,
                lhs.getElement(2, 0)*rhs.x+lhs.getElement(2, 1)*rhs.y+lhs.getElement(2, 2)*rhs.z);
    }
    friend CRTVector operator*(const CRTVector lhs, const CRTMatrix rhs) {
    return 
        CRTVector(lhs.x*rhs.getElement(0, 0) + lhs.y *rhs.getElement(1, 0) + lhs.z *rhs.getElement(2, 0),
                lhs.x*rhs.getElement(0, 1) + lhs.y *rhs.getElement(1, 1) + lhs.z *rhs.getElement(2, 1),
                lhs.x*rhs.getElement(0, 2) + lhs.y *rhs.getElement(1, 2) + lhs.z *rhs.getElement(2, 2));
    }
    friend CRTMatrix operator*(const CRTMatrix& lhs, const CRTMatrix& rhs) {
        CRTVector v0(
            lhs.getElement(0, 0)*rhs.getElement(0,0)+lhs.getElement(0, 1)*rhs.getElement(1,0)+lhs.getElement(0, 2)*rhs.getElement(2,0),
            lhs.getElement(0, 0)*rhs.getElement(0,1)+lhs.getElement(0, 1)*rhs.getElement(1,1)+lhs.getElement(0, 2)*rhs.getElement(2,1),
            lhs.getElement(0, 0)*rhs.getElement(0,2)+lhs.getElement(0, 1)*rhs.getElement(1,2)+lhs.getElement(0, 2)*rhs.getElement(2,2)
        );
        CRTVector v1(
            lhs.getElement(1, 0)*rhs.getElement(0,0)+lhs.getElement(1, 1)*rhs.getElement(1,0)+lhs.getElement(1, 2)*rhs.getElement(2,0),
            lhs.getElement(1, 0)*rhs.getElement(0,1)+lhs.getElement(1, 1)*rhs.getElement(1,1)+lhs.getElement(1, 2)*rhs.getElement(2,1),
            lhs.getElement(1, 0)*rhs.getElement(0,2)+lhs.getElement(1, 1)*rhs.getElement(1,2)+lhs.getElement(1, 2)*rhs.getElement(2,2)
        );
        CRTVector v2(
            lhs.getElement(2, 0)*rhs.getElement(0,0)+lhs.getElement(2, 1)*rhs.getElement(1,0)+lhs.getElement(2, 2)*rhs.getElement(2,0),
            lhs.getElement(2, 0)*rhs.getElement(0,1)+lhs.getElement(2, 1)*rhs.getElement(1,1)+lhs.getElement(2, 2)*rhs.getElement(2,1),
            lhs.getElement(2, 0)*rhs.getElement(0,2)+lhs.getElement(2, 1)*rhs.getElement(1,2)+lhs.getElement(2, 2)*rhs.getElement(2,2)
        );

        return CRTMatrix(v0,v1,v2);
    }


    private:
    float matrix[3][3];
};

#endif