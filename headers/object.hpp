#ifndef CRTOBJECT
#define CRTOBJECT

#include "crtVector.hpp"
#include "matrix.hpp"
#include "mesh.hpp"
class CRTObject {

    public:
        CRTObject() {};
        CRTObject(CRTMesh mesh) : mesh(mesh) {}
        CRTMesh mesh;
        CRTMatrix rotationMatrix;
        CRTVector offset;
    private:

};

#endif