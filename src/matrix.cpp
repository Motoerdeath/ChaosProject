#include "../headers/matrix.hpp"

    CRTMatrix::CRTMatrix() {
        matrix[0][0] = 1.f;
        matrix[0][1] = 0.f;
        matrix[0][2] = 0.f;
        matrix[1][0] = 0.f;
        matrix[1][1] = 1.f;
        matrix[1][2] = 0.f;
        matrix[2][0] = 0.f;
        matrix[2][1] = 0.f;
        matrix[2][2] = 1.f;
    }
    CRTMatrix::CRTMatrix(CRTVector vector1,CRTVector vector2,CRTVector vector3) {
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
    float CRTMatrix::getElement(int row, int column) const {
        return matrix[row][column];
    }
    CRTMatrix CRTMatrix::getRotationMatrixAroundX(float degs) {
        const float rads = degs*(M_PI/180.f);
        return CRTMatrix(
            CRTVector(1.0f,0.0f,0.0f),
            CRTVector(0.0f,std::cos(rads),-std::sin(rads)),
            CRTVector(0.0f,std::sin(rads),std::cos(rads))
        );
    }
    CRTMatrix CRTMatrix::getRotationMatrixAroundY(float degs) {
        const float rads = degs*(M_PI/180.f);
        return CRTMatrix(
            CRTVector(std::cos(rads),0.0f,-std::sin(rads)),
            CRTVector(0.0f,1.0f,0.0f),
            CRTVector(std::sin(rads),0.0f,std::cos(rads))
        );
    }
    CRTMatrix CRTMatrix::getRotationMatrixAroundZ(float degs) {
        const float rads = degs*(M_PI/180.f);
        return CRTMatrix(
            CRTVector(std::cos(rads),-std::sin(rads),0.0f),
            CRTVector(std::sin(rads),std::cos(rads),0.0f),
            CRTVector(0.0f,0.0f,1.0f)
        );
    }

    CRTMatrix CRTMatrix::operator*(const float scalar) {
        return CRTMatrix(CRTVector(matrix[0][0]*scalar,matrix[0][1]*scalar,matrix[0][2]*scalar),
        CRTVector(matrix[1][0]*scalar,matrix[1][1]*scalar,matrix[1][2]*scalar),
        CRTVector(matrix[2][0]*scalar,matrix[2][1]*scalar,matrix[2][2]*scalar));
    }


