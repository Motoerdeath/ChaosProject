#include "../src/Tasks/homework2.cpp"
#include "../src/Tasks/homework3.cpp"
#include "../src/Tasks/homework4.cpp"
#include "../src/Tasks/homework5.cpp"

#include "../headers/triangle.hpp"
#include "../headers/camera.hpp"
#include "glm/geometric.hpp"
#include <cfloat>
#include <cstdint>
#include "../headers/matrix.hpp"

bool FUNDAMENTALS = true;
bool RAYS = true;



int main() {
    /*
    Homework2 homework2;
    homework2.execute();

    Homework3 homework3;
    homework3.execute();

    Homework4 homework4;
    homework4.execute();

    Homework5 homework5;
    homework5.execute();
    */
    CRTMatrix matrix(CRTVector(1.0f),CRTVector(1.0f),CRTVector(1.0f));
    CRTMatrix matrix2(CRTVector(1.0f,0.0f,0.0f),CRTVector(0.0f,1.0f,0.0f),CRTVector(0.f,0.f,1.0f));
    CRTMatrix result = matrix*matrix;
    CRTVector vector(0.f,0.f,-1.f);
    vector = vector * CRTMatrix::getRotationMatrixAroundY(30.f);
    return 0;
}