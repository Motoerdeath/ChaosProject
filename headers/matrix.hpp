#ifndef CRTMACRO
#define CRTMACRO
#include "crtVector.hpp"
class CRTMatrix{
    public:

    CRTMatrix rotate();
    CRTMatrix transform(CRTVector movDir);
    CRTMatrix scale(float scalar);

    CRTMatrix operator*(const float scalar);
    CRTMatrix operator*(const CRTVector vector);
    CRTMatrix operator*(const CRTMatrix matrix);

    private:
    float matrix[3][3];
};

#endif