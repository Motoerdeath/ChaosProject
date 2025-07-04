#ifndef CRTRENDERER
#define CRTRENDERER

#include "crtVector.hpp"
class CRTRenderer {
    public:
    CRTRenderer() {}
    private:
    CRTVector render();
};

class ConstantRenderer : public CRTRenderer {

};

class FlatRenderer : public CRTRenderer {

};


class SmoothRenderer : public CRTRenderer {

};
#endif